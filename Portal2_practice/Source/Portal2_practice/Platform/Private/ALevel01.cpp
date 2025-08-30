// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "ALevel01.h"

#include "AElevator.h"
#include "UPortalEventManager.h"
#include "Kismet/GameplayStatics.h"

ALevel01::ALevel01()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevel01::BeginPlay()
{
	Super::BeginPlay();

	this->InitLevel();
}

void ALevel01::InitLevel()
{
	for (const auto& Group : LightGroup )
	{
		UPortalEventManager::Get(this)->SendLight(Group, false);
	}
	
	TArray<AActor*> FoundActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElevator::StaticClass(), FoundActor);
	for (const auto& Actor : FoundActor)
	{
		if (const auto& Elevator = Cast<AElevator>(Actor))
		{
			if ( Elevator->IsSameGroup(StartElevatorGroup) )
				Elevator->SetMoveState(true);
		}
	}
}

void ALevel01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

