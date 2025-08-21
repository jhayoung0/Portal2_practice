#pragma once

#include "CoreMinimal.h"
#include "Core/Macro.h"
#include "UGameEventManager.generated.h"


UCLASS()
class COFFEELIBRARY_API UGameEventManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	DEFINE_SUBSYSTEM_GETTER_INLINE(UGameEventManager);


	DECLARE_MULTICAST_DELEGATE_OneParam(FOnScore, int);
	FOnScore OnScore;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKill, int, Kill);
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnKill OnKill;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessage, FString, Msg);
	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnMessage OnMessage;
	
	UFUNCTION(BlueprintCallable, Category="Events")
	void SendMessage(const FString& InMsg);
};
