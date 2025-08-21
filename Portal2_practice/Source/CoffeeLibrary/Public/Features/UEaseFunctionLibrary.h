// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UEaseFunctionLibrary.generated.h"

UCLASS()
class COFFEELIBRARY_API UEaseFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Ease", meta=(DisplayName="Ease Alpha (Custom)", Keywords="Ease Easing Curve"))
	static float EaseAlphaCustom(float Alpha, EEaseType Type);

	UFUNCTION(BlueprintPure, Category="Ease", meta=(DisplayName="Lerp (Float, Custom Ease)", Keywords="Lerp Ease"))
	static float LerpFloatEase(float A, float B, float Alpha, EEaseType Type);

	UFUNCTION(BlueprintPure, Category="Ease", meta=(DisplayName="Lerp (Vector, Custom Ease)", Keywords="Lerp Ease"))
	static FVector LerpVectorEase(const FVector& A, const FVector& B, float Alpha, EEaseType Type);
};