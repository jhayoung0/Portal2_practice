// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "ALevel01.generated.h"

UCLASS()
class PORTAL2_PRACTICE_API ALevel01 : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevel01();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="LEVEL01")
	void InitLevel();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LEVEL01");
	TArray<int32> LightGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LEVEL01");
	int32 StartElevatorGroup;
};
