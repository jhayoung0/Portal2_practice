// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "AElevator.h"
#include "APatrolPoint.h"
#include "FirstPersonCharacter.h"
#include "ULog.h"

#include "Features/UCommonFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Shared/FComponentHelper.h"

#define ELEVATOR_COLLISION_PATH		TEXT("EvelatorIn")

#define WALL01_COLLISION_PATH		TEXT("Wall01")
#define WALL02_COLLISION_PATH		TEXT("Wall02")
#define WALL03_COLLISION_PATH		TEXT("Wall03")
#define WALL04_COLLISION_PATH		TEXT("Wall04")

// Sets default values
AElevator::AElevator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AElevator::BeginPlay()
{
	Super::BeginPlay();

	ElevatorCollision = FComponentHelper::FindComponentByNameRecursive<UPrimitiveComponent>(this, ELEVATOR_COLLISION_PATH);

	this->FindWall();
	this->FindStartEndPos();

	TArray<AActor*> OverlappingActors;
	ElevatorCollision->GetOverlappingActors(OverlappingActors);

	// for (AActor* Actor : OverlappingActors)
	// {
	// 	// 특정 타입만 필터링
	// 	auto Player = Cast<AFirstPersonCharacter>(Actor);
	// 	if ( IsValid(Player))
	// 	{
	// 		ULOG(Warning, "ELEVATOR AUTO MOVE");
	// 		this->SetMoveState(true);
	// 		break;
	// 	}
	// }
}

void AElevator::FindWall()
{
	{
		const auto Wall = FComponentHelper::FindComponentByNameRecursive<UPrimitiveComponent>(this, WALL01_COLLISION_PATH);
		WallArray.Push(Wall);
	}
	
	{
		const auto Wall = FComponentHelper::FindComponentByNameRecursive<UPrimitiveComponent>(this, WALL02_COLLISION_PATH);
		WallArray.Push(Wall);
	}
	
	{
		const auto Wall = FComponentHelper::FindComponentByNameRecursive<UPrimitiveComponent>(this, WALL03_COLLISION_PATH);
		WallArray.Push(Wall);
	}
	
	{
		const auto Wall = FComponentHelper::FindComponentByNameRecursive<UPrimitiveComponent>(this, WALL04_COLLISION_PATH);
		WallArray.Push(Wall);
	}
}

void AElevator::FindStartEndPos()
{
	TArray<AActor*> FindActor;
	UGameplayStatics::GetAllActorsOfClass( GetWorld(), APatrolPoint::StaticClass(), FindActor );

	for ( auto Actor : FindActor )
	{
		auto PatrolPoint = Cast<APatrolPoint>(Actor);
		if ( IsValid(PatrolPoint))
		{
			if ( PatrolPoint->IsSameGroup(Group) )
			{
				if ( PatrolPoint->IsStartType() )
				{
					StartPos = PatrolPoint;
				}
				else
				{
					EndPos = PatrolPoint;
				}
			}
		}
	}

	this->SetActorLocation(StartPos->GetActorLocation() );
}

void AElevator::SetWallCollision(const bool State)
{
	for ( auto Wall : WallArray )
	{
		Wall->SetCollisionEnabled(State ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	}
}

void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ( !MoveState )
		return;

	if ( MoveDirection == EElevatorMoveDirectionType::Start_To_End)
	{
		auto Direction = EndPos->GetActorLocation() - this->GetActorLocation();
		Direction.Normalize();

		const FVector TargetPos = this->GetActorLocation() +  Direction * MoveSpeed * DeltaTime;
		this->SetActorLocation( TargetPos );

		const float Distance = UCommonFunctionLibrary::GetDistance( EndPos, this );
		if ( Distance < 0.1f )
		{
			this->SetMoveState(false);
			this->MoveDone();
		}
	}
	else
	{
		auto Direction = StartPos->GetActorLocation() - this->GetActorLocation();
		Direction.Normalize();

		const FVector TargetPos = this->GetActorLocation() +  Direction * MoveSpeed * DeltaTime;
		this->SetActorLocation( TargetPos );

		const float Distance = UCommonFunctionLibrary::GetDistance( StartPos, this );
		if ( Distance < 0.1f )
		{
			this->SetMoveState(false);
			this->MoveDone();
		}
	}
}

void AElevator::MoveDone_Implementation()
{
	ULOG( Warning, "MoveDone" );
}

