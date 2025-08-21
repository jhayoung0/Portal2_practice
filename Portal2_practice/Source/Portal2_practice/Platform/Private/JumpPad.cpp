// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "JumpPad.h"
#include "ULog.h"

#include "Shared/FComponentHelper.h"
#include "Shared/FMaterialHelper.h"
#include "Shared/FMathHelper.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

#include "Misc/OutputDeviceNull.h"

#define SWITCH_BUTTON_PATH			TEXT("Mesh_SwitchButton")
#define SWITCH_COLLISION_PATH		TEXT("Collision")

#define BP_FirstPersonCharacter		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter.BP_FirstPersonCharacter_C")

AJumpPad::AJumpPad()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
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
		OriginVector = SwitchButton->GetRelativeLocation();
		SwitchCollision->OnComponentBeginOverlap.AddDynamic(this, &AJumpPad::OnBeginOverlap);

		MaterialButton->SetVectorParameterValue(ColorParam, WarningColor);
		SwitchButton->SetRelativeLocation(OriginVector);
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
		const auto Color_B = WarningColor;
		const auto Color_Result = UKismetMathLibrary::LinearColorLerpUsingHSV(Color_A, Color_B, AlphaValue);

		MaterialButton->SetVectorParameterValue(ColorParam, Color_Result);
	}

	{
		const auto Vector_A = SwitchButton->GetRelativeLocation();
		const auto Vector_B = OriginVector;
		const auto Vector_Result = UKismetMathLibrary::VLerp(Vector_A, Vector_B, AlphaValue);

		SwitchButton->SetRelativeLocation(Vector_Result);
	}

	if (!bIsJumping || !InOtherActor)
		return;

	// ── FPS 독립: 실제 지난 위치/시간으로 속도 계산 ─────────────────────
	const double Now = GetWorld()->GetTimeSeconds();
	const FVector NewPos = FMathHelper::InterpArcSin(StartPos, EndPos, Height, AlphaValue);
	
	FHitResult Hit;
	InOtherActor->SetActorLocation(NewPos, true, &Hit);

	// Δt 계산 (최소값 클램프)
	const double Dt = FMath::Max(1e-6, Now - LastInterpTime);

	// PrevPos는 '진짜 지난 프레임 위치'
	FVector Velocity = FVector::ZeroVector;
	if (bHasLastInterp)
	{
		Velocity = (NewPos - LastInterpPos) / Dt;
		LastComputedVelocity = Velocity; // 복구용 저장
	}

	// 다음 프레임 대비 갱신
	LastInterpPos = NewPos;
	LastInterpTime = Now;
	bHasLastInterp = true;
	// ────────────────────────────────────────────────────────────────
	
	if (Hit.IsValidBlockingHit() && !bPhysicsRestored )
	{
		// 충돌했다
		InOtherActor->SetActorLocation(LastInterpPos, false);

		RestorePhysicsOrMovement(DeltaTime, AlphaValue, NewPos, LastComputedVelocity);

		bIsJumping = false;
		InOtherActor = nullptr;
		ElapsedTime = 0.f;

		// 다음 점프를 위해 리셋
		bHasLastInterp = false;
		SetActorTickEnabled(false);
		return;
	}

	if (AlphaValue >= 0.85f && !bPhysicsRestored)
	{
		RestorePhysicsOrMovement(DeltaTime, AlphaValue, NewPos, LastComputedVelocity);
	}

	if (AlphaValue >= 1.0f)
	{
		bIsJumping = false;
		InOtherActor = nullptr;
		ElapsedTime = 0.0f;

		// 다음 점프를 위해 리셋
		bHasLastInterp = false;
		SetActorTickEnabled(false);
	}
 }


void AJumpPad::RestorePhysicsOrMovement(float DeltaTime, float AlphaValue, FVector NewPos, const FVector& InVelocity)
{
	const FVector FinalVelocity = [&]()
	{
		if (bUseForcChracterVelocity) return OutCharacterForceVelocity;
		if (bUseForceCubeVelocity)    return OutCubeForceVelocity;
		return InVelocity; // 계산된 근사 속도
	}();
	
	if (ACharacter* Player = Cast<ACharacter>(InOtherActor))
	{
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
		{
			MoveComp->SetMovementMode(MOVE_Falling);
			MoveComp->Velocity = FinalVelocity;

			ULOG(Warning, "Character Velocity: X=%.2f Y=%.2f Z=%.2f | Size=%.2f", FinalVelocity.X, FinalVelocity.Y, FinalVelocity.Z, FinalVelocity.Size());

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
		ULOG(Warning, "Character Velocity: X=%.2f Y=%.2f Z=%.2f | Size=%.2f", FinalVelocity.X, FinalVelocity.Y, FinalVelocity.Z, FinalVelocity.Size());
		
		MeshComp->SetSimulatePhysics(true);
		MeshComp->SetPhysicsLinearVelocity(FinalVelocity);
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
	if (!OtherActor || InOtherActor == OtherActor || bIsJumping || !LandingActor )
		return;

	this->StartPos = OtherActor->GetActorLocation();
	this->EndPos   = LandingActor->GetActorLocation();

	// 도착지가 너무 가까우면 무시
	if (FVector::DistSquared(StartPos, EndPos) < 1.f)
		return;

	this->InOtherActor = OtherActor;
	this->ElapsedTime = 0.0;
	this->bPhysicsRestored = false;
	this->bIsJumping = true;

	this->LastInterpPos = StartPos;
	this->LastInterpTime = GetWorld()->GetTimeSeconds();
	this->bHasLastInterp = false;

	SetActorTickEnabled(true);

	MaterialButton->SetVectorParameterValue(ColorParam, IdleColor);
	SwitchButton->SetRelativeLocation(EndVector);
	
	if (ACharacter* Player = Cast<ACharacter>(OtherActor))
	{
		if (UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement())
			MoveComp->DisableMovement();
	}
	else if (UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(OtherComp))
	{
		AActor* ParentActor = OtherActor->GetAttachParentActor();

		UClass* BPClass = StaticLoadClass(ACharacter::StaticClass(), nullptr, BP_FirstPersonCharacter );
		if (ParentActor && BPClass && ParentActor->IsA(BPClass))
		{
			ACharacter* FirstPersonCharacter = Cast<ACharacter>(ParentActor);
			if( FirstPersonCharacter != nullptr )
			{
				FName FuncName("CanShoot");
				if ( FirstPersonCharacter->FindFunction(FuncName) )
				{
					FOutputDeviceNull Ar;
					
					const FString Command = FString::Printf(TEXT("CanShoot %s"), TEXT("true"));
					FirstPersonCharacter->CallFunctionByNameWithArguments(*Command, Ar, nullptr, true);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("CanShoot function not found on %s"), *FirstPersonCharacter->GetName());
				}
			}
		}
		
		OtherActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		
		if (MeshComp->IsSimulatingPhysics())
			MeshComp->SetSimulatePhysics(false);
	}

	if (bShowLine)
		FMathHelper::DrawParabolaDebug(GetWorld(), StartPos, EndPos, Height, 20, 2.0f, FColor::Red);

	UE_LOG(LogTemp, Warning, TEXT("JumpPad triggered: %s -> %s"), *StartPos.ToString(), *EndPos.ToString());
}