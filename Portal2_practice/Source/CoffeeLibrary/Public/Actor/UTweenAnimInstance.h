// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UTweenAnimInstance.generated.h"

UCLASS()
class COFFEELIBRARY_API UTweenAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	bool bActivate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween", meta=(ClampMin="0.001"))
	float TweenDuration = 0.25f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Tween")
	float TweenAlpha = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tween")
	TObjectPtr<UCurveFloat> EaseCurve = nullptr;
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category="Tween")
	void ChangeState(bool bInActivate) { bActivate = bInActivate; }

	UFUNCTION(BlueprintCallable, Category="Tween")
	void ResetAlpha(float InAlpha = 0.f) { TweenAlpha = FMath::Clamp(InAlpha, 0.f, 1.f); }

	UFUNCTION(BlueprintPure, Category="Tween")
	float GetEasedAlpha() const;
};
