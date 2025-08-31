// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UCLASS()
class PORTAL2_PRACTICE_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	AJumpPad();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void RestorePhysicsOrMovement(float DeltaTime, float AlphaValue, FVector NewPos, const FVector& InVelocity);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="JumpPad")
	void ActivatePad();
	
private:
	void AddElapsedTime();
	float LerpAlpha() const;
	bool ActivateTrigger();
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);	
public:
	float ElapsedTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	float Duration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	float TriggerDelay = 3.0f;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	// FName ColorParam = FName("Color");
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	// FLinearColor IdleColor = FLinearColor::White;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	// FLinearColor WarningColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Switch")
	FVector OriginVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	FVector EndVector;

	// UPROPERTY()
	// class UStaticMeshComponent* SwitchButton;
	// UPROPERTY()
	// class UMaterialInstanceDynamic* MaterialButton;
	UPROPERTY()
	class UPrimitiveComponent* SwitchCollision;
	UPROPERTY()
	class AActor* InOtherActor;

	
	UPROPERTY(EditAnywhere, Category="JumpPad")
	float TotalTime = 1.5f;   // 이동 전체 시간 (초)
	UPROPERTY(EditAnywhere, Category="JumpPad")
	float Height = 300.f;     // 아치 높이 (포물선 최대 높이)
	UPROPERTY(EditAnywhere, Category="JumpPad")
	class AActor* LandingActor;
	UPROPERTY(EditAnywhere, Category="JumpPad")
	bool  bShowLine = false;

	// UPROPERTY(EditAnywhere, Category="JumpPad")
	// bool  bUseForcChracterVelocity  = false;
	// UPROPERTY(EditAnywhere, Category="JumpPad")
	// bool  bUseForceCubeVelocity  = false;
	// UPROPERTY(EditAnywhere, Category="JumpPad")
	// FVector OutCharacterForceVelocity = FVector(0,0,0);
	// UPROPERTY(EditAnywhere, Category="JumpPad")
	// FVector OutCubeForceVelocity = FVector(0,0,0);

private:
	FVector StartPos;         // 시작 좌표
	FVector EndPos;           // 도착 좌표
	bool    bIsJumping = false;
	bool    bPhysicsRestored = false;
	
	FVector LastInterpPos = FVector::ZeroVector;
	double  LastInterpTime = 0.0;
	bool    bHasLastInterp = false;
	FVector LastComputedVelocity = FVector::ZeroVector; // 로깅/복구용
};
