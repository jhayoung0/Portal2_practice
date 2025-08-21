// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "Features/USequenceManager.h"
#include "Features/USequenceActivatable.h"
#include "TimerManager.h"
#include "Engine/World.h"

void USequenceManager::RequestSequential(const TArray<AActor*>& Targets,
										 bool bActivate,
										 float InitialDelay,
										 float Interval,
										 float Duration)
{
	CancelAll();

	Queue.Reset();
	Queue.Reserve(Targets.Num());

	for (AActor* A : Targets)
	{
		if (!IsValid(A))
			continue;
		
		FSequenceCommand Cmd;
		Cmd.Target   = A;
		Cmd.bActivate = bActivate;
		Cmd.Duration = Duration;
		Queue.Add(MoveTemp(Cmd));
	}

	Index = 0;
	StepInterval = FMath::Max(0.f, Interval);

	if (!GetWorld() || Queue.Num() == 0)
		return;

	GetWorld()->GetTimerManager().SetTimer(
		MainHandle,
		this,
		&USequenceManager::TickStep,
		(StepInterval <= KINDA_SMALL_NUMBER) ? 0.0f : StepInterval,  // 0이면 프레임마다 처리
		(StepInterval > KINDA_SMALL_NUMBER),                         // 반복 타이머 여부
		FMath::Max(0.f, InitialDelay)                          		 // 초기 지연
	);

	if (StepInterval <= KINDA_SMALL_NUMBER)
		TickStep();
}

void USequenceManager::TickStep()
{
	if (Queue.Num() == 0)
	{
		CancelAll();
		return;
	}

	const int32 MaxPerTick = (StepInterval > KINDA_SMALL_NUMBER) ? 1 : 8;

	int32 Processed = 0;
	while (Index < Queue.Num() && Processed < MaxPerTick)
	{
		const FSequenceCommand& Cmd = Queue[Index];

		AActor* Target = Cmd.Target.Get();
		if (IsValid(Target) && Target->GetClass()->ImplementsInterface(USequenceActivatable::StaticClass()))
		{
			if (Cmd.bActivate)
			{
				ISequenceActivatable::Execute_Activate(Target, Index, Queue.Num(), Cmd.Duration);
			}
			else
			{
				ISequenceActivatable::Execute_Deactivate(Target, Index, Queue.Num(), Cmd.Duration);
			}
		}

		++Index;
		++Processed;
	}

	if (Index >= Queue.Num())
	{
		CancelAll();
	}
}

void USequenceManager::CancelAll()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(MainHandle);
	}
	Queue.Reset();
	Index = 0;
}