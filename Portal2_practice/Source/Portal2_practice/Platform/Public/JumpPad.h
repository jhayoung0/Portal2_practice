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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	FName ColorParam = FName("Color");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	FLinearColor IdleColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	FLinearColor WarningColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Switch")
	FVector OriginVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Switch")
	FVector EndVector;

	UPROPERTY()
	class UStaticMeshComponent* SwitchButton;
	UPROPERTY()
	class UMaterialInstanceDynamic* MaterialButton;
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

	FVector StartPos;         // 시작 좌표
	FVector EndPos;           // 도착 좌표
	float   Elapsed = 0.f;    // 경과 시간
	bool    bIsJumping = false;
	bool    bPhysicsRestored = false;
};
