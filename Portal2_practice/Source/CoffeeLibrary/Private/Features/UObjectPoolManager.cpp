// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "Features/UObjectPoolManager.h"
#include "Engine/World.h"


void UObjectPoolManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Log, TEXT("ObjectPoolManager Initialized."));
}

void UObjectPoolManager::Deinitialize()
{
	for (auto& Elem : PoolMap)
	{
		for (AActor* Actor : Elem.Value)
		{
			if (IsValid(Actor))
			{
				Actor->Destroy();
			}
		}
	}
	PoolMap.Empty();
    
	Super::Deinitialize();
}

AActor* UObjectPoolManager::GetPoolItem(const UObject* WorldContextObject, const TSubclassOf<AActor> InClass )
{
	return GetPoolItemLocationRotator(WorldContextObject, InClass, FVector::ZeroVector, FRotator::ZeroRotator );
}

AActor* UObjectPoolManager::GetPoolItemLocationRotator(const UObject* WorldContextObject, const TSubclassOf<AActor> InClass, const FVector Location, const FRotator Rotator  )
{
	if (!InClass)
	{
		UE_LOG(LogTemp, Error, TEXT("GetPoolItem failed: InClass is null."));
		return nullptr;
	}
	
	TArray<AActor*>* Pool = PoolMap.Find(InClass);
	if (Pool && Pool->Num() > 0)
	{
		AActor* Actor = Pool->Pop();
		Actor->SetActorLocationAndRotation(Location, Rotator);

		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
		Actor->SetActorTickEnabled(true);
		
		return Actor;
	}

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		return World->SpawnActor<AActor>(InClass, Location, Rotator);

	return nullptr;
}

void UObjectPoolManager::ReturnActorToPool(AActor* Actor)
{
	if ( Actor == nullptr )
		return;
        
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);

	TSubclassOf<AActor> ActorClass = Actor->GetClass();
	TArray<AActor*>& Pool = PoolMap.FindOrAdd(ActorClass);
	Pool.Add(Actor);
}