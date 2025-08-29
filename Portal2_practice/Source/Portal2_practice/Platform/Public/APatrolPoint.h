// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "ScreenPass.h"
#include "GameFramework/Actor.h"
#include "APatrolPoint.generated.h"

UENUM(BlueprintType)
enum class EPatrolPointType : uint8
{
	Start           UMETA(DisplayName="Start Point"),
	End				UMETA(DisplayName="End Point")
};

UCLASS()
class PORTAL2_PRACTICE_API APatrolPoint : public AActor
{
	GENERATED_BODY()

public:
	APatrolPoint();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Patrol")
	FORCEINLINE bool IsStartType() const
	{
		return PointType == EPatrolPointType::Start;
	}

	UFUNCTION(BlueprintCallable, Category="Patrol")
	FORCEINLINE bool IsSameGroup(int32 InGroup) const
	{
		return Group == InGroup;
	}

private:
	UPROPERTY(EditAnywhere, Category="Partrol")
	int32 Group;

	UPROPERTY(EditAnywhere, Category="Partrol")
	EPatrolPointType PointType;
};
