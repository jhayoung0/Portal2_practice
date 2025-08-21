// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UListActorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COFFEELIBRARY_API UListActorComponent : public UActorComponent
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
