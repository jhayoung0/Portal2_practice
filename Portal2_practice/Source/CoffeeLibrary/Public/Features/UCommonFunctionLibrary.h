// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UCommonFunctionLibrary.generated.h"

UCLASS()
class COFFEELIBRARY_API UCommonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// static void TestULog();
	// static void TestInBound();

	UFUNCTION(BlueprintPure, Category = "CoffeeLibrary|Utilities", meta = (DisplayName = "InBounds"))
	static bool InBounds(const int32 Index, const int32 Count);

	UFUNCTION(BlueprintPure, Category="Utils|Array", meta=(CompactNodeTitle="RandElem", ArrayParm="TargetArray", ArrayTypeDependentParams="ReturnValue"))
	static int32 GetRandomIndex(const TArray<int32>& TargetArray, bool& bIsValid);
	
	UFUNCTION(BlueprintPure, Category = "CoffeeLibrary|Utilities", meta = (DisplayName = "NowTimestamp"))
	static int64 GetNowTimestamp();	
	
	UFUNCTION(BlueprintPure, Category = "CoffeeLibrary|Utilities", meta = (DisplayName = "GetFirstNumberInActorLabel"))
	static int32 GetFirstNumberInActorLabel(const AActor* Actor);

	UFUNCTION(BlueprintPure, Category="CoffeeLibrary|Utilities", meta=(DefaultToSelf="Target", DisplayName="GetDistance"))
	static float GetDistance( AActor* A,  AActor* B);
	
	UFUNCTION(BlueprintCallable, Category="CoffeeLibrary|Material", meta=(DefaultToSelf="Target", DisplayName="Get Or Create MID", AdvancedDisplay="OptionalName"))
	static class UMaterialInstanceDynamic* GetOrCreateMID(
		class UPrimitiveComponent* Target,
		int32 ElementIndex,
		FName OptionalName = NAME_None);

	UFUNCTION(BlueprintCallable, Category="CoffeeLibrary|Utilities", meta=(DefaultToSelf="Target", DisplayName="PlayCommonSound"))
	static void PlayLocationSound(const AActor* Actor, USoundBase* Sound, const float RetriggerDelay);
};

