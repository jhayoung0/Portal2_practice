// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "USequenceManager.generated.h"

USTRUCT()
struct COFFEELIBRARY_API FSequenceCommand
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AActor> Target;

	UPROPERTY()
	bool bActivate = true;

	UPROPERTY()
	float Duration = 0.f;
};

UCLASS()
class COFFEELIBRARY_API USequenceManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	/** 0.1초 간격으로 Targets를 순차 Activate/Deactivate
	 *  InitialDelay: 첫 실행 지연
	 *  Interval    : 대상 간 간격(예: 0.1f)
	 *  Duration    : Activate 시 각 대상에 전달되는 연출 길이
	 */
	UFUNCTION(BlueprintCallable, Category="Sequence")
	void RequestSequential(const TArray<AActor*>& Targets,
						   bool bActivate,
						   float InitialDelay = 0.f,
						   float Interval = 0.1f,
						   float Duration = 0.5f);

	UFUNCTION(BlueprintCallable, Category="Sequence")
	void CancelAll();

private:
	void TickStep();

private:
	FTimerHandle MainHandle;
	TArray<FSequenceCommand> Queue;
	int32 Index = 0;
	float StepInterval = 0.1f;
};