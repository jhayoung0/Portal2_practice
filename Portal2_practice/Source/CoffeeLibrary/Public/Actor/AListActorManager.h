// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AListActorManager.generated.h"

UCLASS()
class COFFEELIBRARY_API AListActorManager : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="List")
	TArray<AActor*> ArrayActors;

	UPROPERTY(EditAnywhere, Category="List")
	TSubclassOf<AActor> ClassFilter;	

	UFUNCTION(CallInEditor, Category="List", meta=(DevelopmentOnly, DisplayName="Assign Item (Replace)"))
	void AssignItemReplace();

	UFUNCTION(CallInEditor, Category="List", meta=(DevelopmentOnly, DisplayName="Assign Item (Append)"))
	void AssignItemAppend();

	UFUNCTION(CallInEditor, Category="List", meta=(DevelopmentOnly, DisplayName="Sort by Name (Asc)"))
	void SortByNameAsc();

	UFUNCTION(CallInEditor, Category="List", meta=(DevelopmentOnly, DisplayName="Sort by Name (Desc)"))
	void SortByNameDesc();

#if WITH_EDITOR
protected:
	int32 GatherSelectedItem(TArray<AActor*>& Out) const;
	void AssignInternal(bool bAppend);
#endif
};
