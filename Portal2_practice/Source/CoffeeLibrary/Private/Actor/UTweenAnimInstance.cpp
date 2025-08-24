// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "Actor/UTweenAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UTweenAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	const float AlphaStep  = DeltaSeconds / TweenDuration;
	const float Dir        = bActivate ? 1.f : -1.f;

	TweenAlpha = FMath::Clamp(TweenAlpha + AlphaStep * Dir, 0.f, 1.f);
}

float UTweenAnimInstance::GetEasedAlpha() const
{
	if (EaseCurve)
		return EaseCurve->GetFloatValue(TweenAlpha);
	return TweenAlpha;
}