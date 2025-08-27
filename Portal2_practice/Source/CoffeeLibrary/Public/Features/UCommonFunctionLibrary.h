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
	
	/**
	 * 인덱스가 유효한 범위 (0, Count) 내에 있는지 확인합니다.
	 * @param Index 검사할 인덱스
	 * @param Count 배열 또는 리스트의 총 길이 (상한값, 미포함)
	 */
	UFUNCTION(BlueprintPure, Category = "CoffeeLibrary|Utilities", meta = (DisplayName = "InBounds"))
	static bool InBounds(const int32 Index, const int32 Count);
	
	UFUNCTION(BlueprintPure, Category = "CoffeeLibrary|Utilities", meta = (DisplayName = "NowTimestamp"))
	static int64 GetNowTimestamp();	
	
	UFUNCTION(BlueprintPure, Category = "CoffeeLibrary|Utilities", meta = (DisplayName = "GetFirstNumberInActorLabel"))
	static int32 GetFirstNumberInActorLabel(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Material", meta=(DefaultToSelf="Target", DisplayName="Get Or Create MID", AdvancedDisplay="OptionalName"))
	static class UMaterialInstanceDynamic* GetOrCreateMID(
		class UPrimitiveComponent* Target,
		int32 ElementIndex,
		FName OptionalName = NAME_None);
};

