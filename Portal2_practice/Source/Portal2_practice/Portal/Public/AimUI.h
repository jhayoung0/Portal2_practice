// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AimUI.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PORTAL2_PRACTICE_API UAimUI : public UUserWidget
{
	GENERATED_BODY()


public:
		
	UFUNCTION(BlueprintImplementableEvent, Category= "Portal")
	void OnOrangeFired();

	UFUNCTION(BlueprintImplementableEvent, Category= "Portal")
	void OnBlueFired();
};
