// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Core/Macro.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UPortalEventManager.generated.h"

UCLASS()
class PORTAL2_PRACTICE_API UPortalEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	DEFINE_SUBSYSTEM_GETTER_INLINE(UPortalEventManager);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessage, FString, Msg);
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnMessage OnMessage;
	
	UFUNCTION(BlueprintCallable, Category="Events")
	void SendMessage(const FString& InMsg);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLight, int, Group, bool, State);
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnLight OnLight;

	UFUNCTION(BlueprintCallable, Category="Events")
	void SendLight(const int& InGroup, bool InState);


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMovie, int, Group, bool, State);
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnMovie OnMovie;

	UFUNCTION(BlueprintCallable, Category="Events")
	void SendMovie(const int& InGroup, bool InState);
};
