#pragma once

#include "CoreMinimal.h"
#include "CoffeeMacro.h"
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
};
