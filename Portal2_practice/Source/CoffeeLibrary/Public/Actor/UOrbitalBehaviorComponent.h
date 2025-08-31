// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UOrbitalBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EOrbitalState : uint8
{
	PlayerFollow UMETA(DisplayName="Player Follow"),
	TargetFloat  UMETA(DisplayName="Target Float"),
	AscendAway   UMETA(DisplayName="Ascend & Vanish"),
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COFFEELIBRARY_API UOrbitalBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOrbitalBehaviorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="State")
	EOrbitalState State = EOrbitalState::PlayerFollow;

	// 상태1
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Follow")
	AActor* PlayerActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Follow")
	float OrbitRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Follow")
	float OrbitSpeedDegPerSec = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Follow")
	float HeightOffset = 50.f;

	// 상태2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float")
	AActor* TargetActor = nullptr; // 지정 위치 대신 액터/컴포넌트로 앵커
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float")
	float BobAmplitude = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float")
	float BobSpeed = 1.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float")
	float IdleYawSpeedDegPerSec = 30.f;

	// 상태3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ascend")
	float AscendSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ascend")
	float AscendDistance = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ascend")
	bool  bDestroyOnVanish = false; // 아니면 Hidden 처리

	UFUNCTION(BlueprintCallable, Category="State")
	void SetState(EOrbitalState NewState);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float AngleDeg = 0.f;
	float TimeAcc  = 0.f;
	FVector AnchorLoc = FVector::ZeroVector;
	float   AscendTravel = 0.f;

	void Tick_PlayerFollow(float DeltaTime);
	void Tick_TargetFloat(float DeltaTime);
	void Tick_AscendAway(float DeltaTime);
};
