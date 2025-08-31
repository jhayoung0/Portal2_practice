// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "ALevel01.h"

#include "AElevator.h"
#include "APatrolPoint.h"
#include "UPortalEventManager.h"
#include "Features/UCommonFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Shared/FComponentHelper.h"

#define STARTGAME_CUE			TEXT("/Game/CustomContents/Platfrom/Sound/Cue/StartGame_Cue.StartGame_Cue")

ALevel01::ALevel01()
{
	PrimaryActorTick.bCanEverTick = true;

	StartGameSound =  FComponentHelper::LoadAsset<USoundBase>(STARTGAME_CUE);
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
	
	auto* PortalEventManager = UPortalEventManager::Get(this);
	if (IsValid(PortalEventManager))
	{
		PortalEventManager->OnLight.AddDynamic(this, &ALevel01::OnLight);
		PortalEventManager->OnMessage.AddDynamic(this, &ALevel01::OnMessage );
	}
}

void ALevel01::StartGame()
{
	if ( IsGameStart )
	{
		return;
	}

	IsGameStart =  true;

	{
		TArray<AActor*> FoundActor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AElevator::StaticClass(), FoundActor);
		for (const auto& Actor : FoundActor)
		{
			if (const auto& Elevator = Cast<AElevator>(Actor))
			{
				if ( Elevator->IsSameGroup(StartElevatorGroup) )
					Elevator->SetMoveState(true);

				if ( Elevator->IsSameGroup(1) )
				{
					auto Position = Elevator->GetActorLocation();
					Elevator->SetActorLocation( FVector(Position.X, Position.Y, 605.0f));
				}
			}
		}
	}

	{
		TArray<AActor*> FoundActor2;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), FoundActor2);
		for (const auto& Actor : FoundActor2)
		{
			if (const auto& Point = Cast<APatrolPoint>(Actor))
			{
				if ( Point->IsSameGroup(1) &&  Point->IsStartType())
				{
					auto Position = Point->GetActorLocation();
					Point->SetActorLocation( FVector(Position.X, Position.Y, 605.0f));
				}
			}
		}
	}
	
	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		UCommonFunctionLibrary::PlayLocationSound(Player, StartGameSound, 0.05f);
}

void ALevel01::OnLight(int32 Group, bool State)
{
	LightGroupState.FindOrAdd(Group) = State;

	int ActivateCount = 0;

	for (auto KV : LightGroupState)
	{
		if( KV.Value )
			ActivateCount++;
	}

	UPortalEventManager::Get(this)->SendDoor(GoalDoorGroup,  ActivateCount == LightGroup.Num() );
}

void ALevel01::OnMessage( FString InMsg )
{
	if( InMsg == StartGameMessage )
	{
		StartGame();
	}
}

void ALevel01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}