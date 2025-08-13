// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "PlatformSwitch.h"
#include "CoffeeLibrary.h"
#include "ULog.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"

#define JUMP_DIRECTION_PATH			TEXT("Arrow_JumpDirection")
#define SWITCH_BUTTON_PATH			TEXT("Mesh_SwitchButton")
#define SWITCH_COLLISION_PATH		TEXT("Collision")

#define PLAYER_TAG					FName("Player")


APlatformSwitch::APlatformSwitch()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlatformSwitch::BeginPlay()
{
	Super::BeginPlay();

	this->TriggerDelay = Duration;
	this->DetectTarget = false;
	this->ElapsedTime = 0;

	JumpDirection = UCoffeeCommonUtil::FindComponentByNameRecursive<UArrowComponent>(this, JUMP_DIRECTION_PATH);
	SwitchButton = UCoffeeCommonUtil::FindComponentByNameRecursive<UStaticMeshComponent>(this, SWITCH_BUTTON_PATH);
	SwitchCollision = UCoffeeCommonUtil::FindComponentByNameRecursive<UPrimitiveComponent>(this, SWITCH_COLLISION_PATH);

	if( SwitchButton != nullptr)
	{
		MaterialButton = SwitchButton->CreateDynamicMaterialInstance(0);
		MaterialButton->SetVectorParameterValue( ColorParam, IdleColor );
		OriginVector = SwitchButton->GetRelativeLocation();

		SwitchCollision->OnComponentBeginOverlap.AddDynamic(this, &APlatformSwitch::OnBeginOverlap);
		SwitchCollision->OnComponentEndOverlap.AddDynamic(this, &APlatformSwitch::OnEndOverlap);
	}
}

void APlatformSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( DetectTarget == false)
		return;

	AddElapsedTime();

	const auto AlphaValue = LerpAlpha();
	// ULOG(Warning, "LerpAlpha : %f", LerpAlpha() );

	{
		const auto Color_A = GetVectorParameterValue(MaterialButton, ColorParam);
		const auto Color_B = WarningColor;
		const auto Color_Result = UKismetMathLibrary::LinearColorLerpUsingHSV(Color_A, Color_B, AlphaValue);
		
		MaterialButton->SetVectorParameterValue( ColorParam, Color_Result );
	}

	{
		const auto Vector_A = SwitchButton->GetRelativeLocation();
		const auto Vector_B = EndVector;
		const auto Vector_Result = UKismetMathLibrary::VLerp( Vector_A, Vector_B, AlphaValue);

		SwitchButton->SetRelativeLocation(Vector_Result);
	}

	if( ActivateTrigger() )
	{
		if ( InOtherActor->Tags.Contains(PLAYER_TAG) )
		{
			JumpCharacter(InOtherActor);
		}
		else
		{
			JumpPhysics(InOtherActor);
		}
	}
}

void APlatformSwitch::AddElapsedTime()
{
	ElapsedTime += GetWorld()->GetDeltaSeconds();
}

float APlatformSwitch::LerpAlpha() const
{
	return UKismetMathLibrary::FClamp( ElapsedTime / Duration, 0.0f, 1.0f );
}

bool APlatformSwitch::ActivateTrigger()
{
	TriggerDelay -= GetWorld()->GetDeltaSeconds();

	if ( TriggerDelay < 0 )
	{
		TriggerDelay = Duration;
		return true;
	}

	return false;
}

FLinearColor APlatformSwitch::GetVectorParameterValue( UMaterialInstanceDynamic* MaterialInstance, const FName& ParamName ) const
{
	if ( MaterialInstance == nullptr )
	{
		UE_LOG(LogTemp, Warning, TEXT("GetVectorParameterValueSafe: MaterialInstance is null"));
		return FLinearColor::Black;
	}
	
	return MaterialInstance->K2_GetVectorParameterValue(ParamName);
}

void APlatformSwitch::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	this->InOtherActor = OtherActor;

	this->DetectTarget = true;
	this->ElapsedTime = 0.0;
}

void APlatformSwitch::OnEndOverlap(
	UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	this->InOtherActor = nullptr;

	this->DetectTarget = false;
	this->ElapsedTime = 0.0;

	MaterialButton->SetVectorParameterValue( ColorParam, IdleColor );
	SwitchButton->SetRelativeLocation(OriginVector);
}

#pragma region JUMP
void APlatformSwitch::JumpCharacter(AActor* TargetActor) const
{
	ACharacter* Player = Cast<ACharacter>(TargetActor);

	if ( Player != nullptr )
		Player->LaunchCharacter(JumpDirection->GetForwardVector() * JumpPower, false, false);
}

void APlatformSwitch::JumpPhysics(AActor* TargetActor) const
{
	UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(TargetActor->GetRootComponent());

	if ( Comp != nullptr )
		Comp->AddImpulse(JumpDirection->GetForwardVector() * JumpPower, NAME_None, true);
}
#pragma endregion