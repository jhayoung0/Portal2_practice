// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "JumpPad.h"
#include "ComponentHelper.h"
#include "FMathHelper.h"
#include "FMaterialHelper.h"

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

	SwitchButton = ComponentHelper::FindComponentByNameRecursive<UStaticMeshComponent>(this, SWITCH_BUTTON_PATH);
	SwitchCollision = ComponentHelper::FindComponentByNameRecursive<UPrimitiveComponent>(this, SWITCH_COLLISION_PATH);

	if (SwitchButton != nullptr)
	{
		MaterialButton = SwitchButton->CreateDynamicMaterialInstance(0);
		MaterialButton->SetVectorParameterValue(ColorParam, IdleColor);
		OriginVector = SwitchButton->GetRelativeLocation();

		SwitchCollision->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnBeginOverlap);
	}
}

void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsJumping || !InOtherActor)
		return;

	AddElapsedTime();
	
	const auto AlphaValue = LerpAlpha();
	// ULOG(Warning, "LerpAlpha : %f", LerpAlpha() );

	{
		const auto Color_A = FMaterialHelper::GetVectorParameterValueSafe(MaterialButton, ColorParam);
		const auto Color_B = IdleColor;
		const auto Color_Result = UKismetMathLibrary::LinearColorLerpUsingHSV(Color_A, Color_B, AlphaValue);

		MaterialButton->SetVectorParameterValue(ColorParam, Color_Result);
	}

	{
		const auto Vector_A = SwitchButton->GetRelativeLocation();
		const auto Vector_B = OriginVector;
		const auto Vector_Result = UKismetMathLibrary::VLerp(Vector_A, Vector_B, AlphaValue);

		SwitchButton->SetRelativeLocation(Vector_Result);
	}

	// 궤적 이동 (0.0 ~ 1.0)
	FVector NewPos = FMathHelper::InterpArcSin(StartPos, EndPos, Height, AlphaValue);
	InOtherActor->SetActorLocation(NewPos, true);

	// 거의 도착했을 때 → 물리/이동 복구
	if (AlphaValue >= 0.85f && !bPhysicsRestored)
	{
		FVector PrevPos = FMathHelper::InterpArcSin(StartPos, EndPos, Height, AlphaValue - 0.01f);
		FVector Velocity = (NewPos - PrevPos) / DeltaTime; // 근사 속도

		if (auto Player = Cast<ACharacter>(InOtherActor))
		{
			if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
			{
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
		else if (auto MeshComp = InOtherActor->FindComponentByClass<UStaticMeshComponent>())
		{
			MeshComp->SetSimulatePhysics(true);
		}

		bPhysicsRestored = true;
	}

	if (AlphaValue >= 1.0f)
	{
		bIsJumping = false;
		InOtherActor = nullptr;
		Elapsed = 0.0f;
	}
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
	
	if (auto* Player = Cast<ACharacter>(OtherActor))
	{
		StartPos	= InOtherActor->GetActorLocation();
		EndPos		= LandingActor->GetActorLocation();
		bIsJumping = true;

		// 이동 중 물리 끄기
		if (auto MoveComp = Player->GetCharacterMovement())
			MoveComp->DisableMovement();
	
		if (bShowLine)
			FMathHelper::DrawParabolaDebug(GetWorld(), StartPos, EndPos, Height, 20, 2.0f, FColor::Red);
	}
	else if (auto* MeshComp = Cast<UStaticMeshComponent>(OtherComp))
	{
		if (MeshComp->IsSimulatingPhysics())
		{
			StartPos   = InOtherActor->GetActorLocation(); //MeshComp->GetComponentLocation();
			EndPos     = LandingActor->GetActorLocation();
			bIsJumping = true;

			// 이동 중 물리 끄기
			MeshComp->SetSimulatePhysics(false);
	
			if (bShowLine)
				FMathHelper::DrawParabolaDebug(GetWorld(), StartPos, EndPos, Height, 20, 2.0f, FColor::Red);
		}
	}
}
