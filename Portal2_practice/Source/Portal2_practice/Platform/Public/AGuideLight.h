// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AGuideLight.generated.h"

UCLASS()
class PORTAL2_PRACTICE_API AGuideLight : public AActor
{
	GENERATED_BODY()

public:
	AGuideLight();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Guide")
	FORCEINLINE float GetActivateValue()
	{
		return ActivateState ? 1.0f : 0.0f;
	}

	UFUNCTION(BlueprintCallable, Category="Guide")
	FORCEINLINE void UpdateLight()
	{
		if ( IsValid( LightMaterial ) )
			LightMaterial->SetScalarParameterValue( ActivateParam, GetActivateValue() );
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guide")
	class UMaterialInstanceDynamic* LightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guide")
	int32 Group;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guide")
	bool ActivateState = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Guide")
	FName ActivateParam = FName("ActivateState");
};