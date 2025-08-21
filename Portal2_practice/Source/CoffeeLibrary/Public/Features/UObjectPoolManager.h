// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Core/Macro.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UObjectPoolManager.generated.h"

UCLASS()
class COFFEELIBRARY_API UObjectPoolManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:	
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    DEFINE_SUBSYSTEM_GETTER_INLINE(UObjectPoolManager);
    
    UFUNCTION(BlueprintCallable, Category="Pool", meta = (WorldContext = "WorldContextObject"))
    AActor* GetPoolItem(const UObject* WorldContextObject, TSubclassOf<AActor> InClass);

    UFUNCTION(BlueprintCallable, Category="Pool", meta = (WorldContext = "WorldContextObject"))
    AActor* GetPoolItemLocationRotator(const UObject* WorldContextObject, const TSubclassOf<AActor> InClass, const FVector Location, const FRotator Rotator );
    
    UFUNCTION(BlueprintCallable, Category="Pool")
    void ReturnActorToPool(AActor* Actor);

private:	
    TMap<UClass*, TArray<AActor*>> PoolMap;
};