// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "FEaseHelper.h"
#include "Components/ActorComponent.h"
#include "UEaseComponent.generated.h"

USTRUCT(BlueprintType)
struct FEaseFloatTrack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEaseType EaseType = EEaseType::Linear;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Current = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Start = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Target = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 1.f;
	float ElapsedTime = 0.f;

	float GetAlpha() const
	{
		return FMath::Clamp(Duration > 0.f ? ElapsedTime / Duration : 1.f, 0.f, 1.f);
	}

	void Update(float DeltaTime)
	{
		ElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(Duration > 0.f ? ElapsedTime / Duration : 1.f, 0.f, 1.f);
		Current = FMath::Lerp(Start, Target, FEaseHelper::Ease(Alpha, EaseType));
	}
};

USTRUCT(BlueprintType)
struct FEaseVectorTrack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEaseType EaseType = EEaseType::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Current = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Start = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Target = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 1.f;

	float ElapsedTime = 0.f;

	float GetAlpha() const
	{
		return FMath::Clamp(Duration > 0.f ? ElapsedTime / Duration : 1.f, 0.f, 1.f);
	}

	void Update(float DeltaTime)
	{
		ElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(Duration > 0.f ? ElapsedTime / Duration : 1.f, 0.f, 1.f);
		Current = FMath::Lerp(Start, Target, FEaseHelper::Ease(Alpha, EaseType));
	}
};

USTRUCT(BlueprintType)
struct FEaseRotatorTrack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEaseType EaseType = EEaseType::Linear;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Current = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Start =  FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Target = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 1.f;

	float ElapsedTime = 0.f;

	float GetAlpha() const
	{
		return FMath::Clamp(Duration > 0.f ? ElapsedTime / Duration : 1.f, 0.f, 1.f);
	}

	void Update(float DeltaTime)
	{
		ElapsedTime += DeltaTime;
		float Alpha = FMath::Clamp(Duration > 0.f ? ElapsedTime / Duration : 1.f, 0.f, 1.f);
		Current = FMath::Lerp(Start, Target, FEaseHelper::Ease(Alpha, EaseType));
	}
};


UCLASS( ClassGroup=(CoffeeLibrary), meta=(BlueprintSpawnableComponent, DisplayName="EaseComponent") )
class COFFEELIBRARY_API UEaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEaseComponent();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UpdateTrack(float DeltaTime);

	UFUNCTION(BlueprintPure, Category="Track")
	float GetTrackAlpha(FName TrankName) const;

	UFUNCTION(BlueprintPure, Category="Track")
	float GetEaseFloatTrack(FName TrackName);
	UFUNCTION(BlueprintCallable, Category="Track")
	void SetEaseFloatTrack(FName TrackName, EEaseType EaseType, float Start, float Target, float Duration = 1.f);
    
	UFUNCTION(BlueprintPure, Category="Track")
	FVector GetEaseVectorTrack(FName TrackName);
	UFUNCTION(BlueprintCallable, Category="Track")
	void SetEaseVectorTrack(FName TrackName, EEaseType EaseType, FVector Start, FVector Target, float Duration = 1.f);

	UFUNCTION(BlueprintPure, Category="Track")
	FRotator GetEaseRotatorTrack(FName TrackName);
	UFUNCTION(BlueprintCallable, Category="Track")
	void SetEaseRotatorTrack(FName TrackName, EEaseType EaseType, FRotator Start, FRotator Target, float Duration = 1.f);

	
private:
	TMap<FName, FEaseFloatTrack> FloatTracks;
	TMap<FName, FEaseVectorTrack> VectorTracks;
	TMap<FName, FEaseRotatorTrack> RotatorTracks;
};
