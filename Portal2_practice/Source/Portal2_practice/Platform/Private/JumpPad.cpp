// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "JumpPad.h"
#include "FComponentHelper.h"
#include "FMaterialHelper.h"
#include "FMathHelper.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

#define SWITCH_BUTTON_PATH			TEXT("Mesh_SwitchButton")
#define SWITCH_COLLISION_PATH		TEXT("Collision")

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AJumpPad::BeginPlay()
{
	Super::BeginPlay();

	this->TriggerDelay = Duration;
	this->ElapsedTime = 0;

	SwitchButton = FComponentHelper::FindComponentByNameRecursive<UStaticMeshComponent>(this, SWITCH_BUTTON_PATH);
	SwitchCollision = FComponentHelper::FindComponentByNameRecursive<UPrimitiveComponent>(this, SWITCH_COLLISION_PATH);

	if (SwitchButton != nullptr)
	{
		MaterialButton = SwitchButton->CreateDynamicMaterialInstance(0);
		MaterialButton->SetVectorParameterValue(ColorParam, IdleColor);
		OriginVector = SwitchButton->GetRelativeLocation();

		SwitchCollision->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnBeginOverlap);
		// SwitchCollision->OnComponentEndOverlap.AddDynamic(this, &AJumpPad::OnEndOverlap);
	}
}

void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddElapsedTime();
	
	auto AlphaValue = LerpAlpha();
	// ULOG(Warning, "LerpAlpha : %f", LerpAlpha() );

	{
		const auto Color_A = FMaterialHelper::GetVectorParameterValueSafe(MaterialButton, ColorParam);
		const auto Color_B = IdleColor;
		const auto Color_Result = UKismetMathLibrary::LinearColorLerpUsingHSV(Color_A, Color_B, AlphaValue);

		MaterialButton->SetVectorParameterValue(ColorParam, Color_Result);
	}

	{
		const auto Vector_A = SwitchButton->GetRelativeLocation();
		const auto Vector_B = EndVector;
		const auto Vector_Result = UKismetMathLibrary::VLerp(Vector_A, Vector_B, AlphaValue);

		SwitchButton->SetRelativeLocation(Vector_Result);
	}

	if (!bIsJumping || !InOtherActor)
		return;
	
	// 궤적 이동 (0.0 ~ 1.0)
	FVector PrevPos = FMathHelper::InterpArcSin(StartPos, EndPos, Height, AlphaValue - 0.01f);
	FVector NewPos  = FMathHelper::InterpArcSin(StartPos, EndPos, Height, AlphaValue);

	FHitResult Hit;
	InOtherActor->SetActorLocation(NewPos, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		// 충돌했다 → 점프 강제 종료
		InOtherActor->SetActorLocation(PrevPos, false);

		RestorePhysicsOrMovement(DeltaTime, AlphaValue, NewPos);

		bIsJumping = false;
		InOtherActor = nullptr;
		ElapsedTime = 0.f;
		return;
	}

	if (AlphaValue >= 0.85f && !bPhysicsRestored)
	{
		// 거의 도착했을 때 → 물리/이동 복구
		RestorePhysicsOrMovement(DeltaTime, AlphaValue, NewPos);
	}

	// 끝났으면 상태 초기화
	if (AlphaValue >= 1.0f)
	{
		bIsJumping = false;
		InOtherActor = nullptr;
		Elapsed = 0.0f;
	}
 }


void AJumpPad::RestorePhysicsOrMovement(float DeltaTime, float AlphaValue, FVector NewPos )
{
	if (ACharacter* Player = Cast<ACharacter>(InOtherActor))
	{
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			FVector PrevPos = FMathHelper::InterpArcSin(StartPos, EndPos, Height, AlphaValue - 0.01f);
			FVector Velocity = (NewPos - PrevPos) / DeltaTime; // 근사 속도
			
			MoveComp->SetMovementMode(MOVE_Falling); // 점프 상태
			MoveComp->Velocity = Velocity;

			// 1초 뒤에 Walking 모드로 복구
			FTimerHandle TimerHandle_ResetMovement;
			Player->GetWorldTimerManager().SetTimer(
				TimerHandle_ResetMovement,
				[MoveComp]()
				{
					if (IsValid(MoveComp) && MoveComp->MovementMode == MOVE_Falling)
					{
						MoveComp->SetMovementMode(MOVE_Walking);
					}
				},
				0.5f,
				false
			);
		}
	}
	else if (UStaticMeshComponent* MeshComp = InOtherActor->FindComponentByClass<UStaticMeshComponent>())
	{
		MeshComp->SetSimulatePhysics(true);
	}

	bPhysicsRestored = true;
}

void AJumpPad::AddElapsedTime()
{
	ElapsedTime += GetWorld()->GetDeltaSeconds();
}

float AJumpPad::LerpAlpha() const
{
	return UKismetMathLibrary::FClamp(ElapsedTime / Duration, 0.0f, 1.0f);
}

bool AJumpPad::ActivateTrigger()
{
	TriggerDelay -= GetWorld()->GetDeltaSeconds();

	if (TriggerDelay < 0)
	{
		TriggerDelay = Duration;
		return true;
	}

	return false;
}

void AJumpPad::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (InOtherActor == OtherActor)
		return;
	
	this->InOtherActor = OtherActor;
	this->ElapsedTime = 0.0;
	this->bPhysicsRestored = false;

	MaterialButton->SetVectorParameterValue(ColorParam, WarningColor);
	SwitchButton->SetRelativeLocation(EndVector);
	
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		StartPos = Player->GetActorLocation();
		EndPos = LandingActor->GetActorLocation(); // 미리 배치한 도착 지점
		bIsJumping = true;

		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
			MoveComp->DisableMovement();
	
		if (bShowLine)
			FMathHelper::DrawParabolaDebug(GetWorld(), StartPos, EndPos, Height, 20, 2.0f, FColor::Red);
	}
	else if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(OtherComp))
	{
		// 물리 큐브 처리
		if (MeshComp->IsSimulatingPhysics())
		{
			StartPos   = MeshComp->GetComponentLocation();
			EndPos     = LandingActor->GetActorLocation();
			bIsJumping = true;
	
			MeshComp->SetSimulatePhysics(false); // 이동 중 물리 끄기
	
			if (bShowLine)
				FMathHelper::DrawParabolaDebug(GetWorld(), StartPos, EndPos, Height, 20, 2.0f, FColor::Red);
		}
	}
}