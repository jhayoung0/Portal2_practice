// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.


#include "Features/UEaseFunctionLibrary.h"
#include "Shared/FEaseHelper.h"

static FORCEINLINE float ApplyEase(float Alpha, EEaseType Type)
{
	return FEaseHelper::Ease(Alpha, Type);
}

float UEaseFunctionLibrary::EaseAlphaCustom(float Alpha, EEaseType Type)
{
	return ApplyEase(Alpha, Type);
}

float UEaseFunctionLibrary::LerpFloatEase(float A, float B, float Alpha, EEaseType Type)
{
	const float T = ApplyEase(Alpha, Type);
	return FMath::Lerp(A, B, T);
}

FVector UEaseFunctionLibrary::LerpVectorEase(const FVector& A, const FVector& B, float Alpha, EEaseType Type)
{
	const float T = ApplyEase(Alpha, Type);
	return FMath::Lerp(A, B, T);
}