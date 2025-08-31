// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UOrbitalBehaviorComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COFFEELIBRARY_API UOrbitalBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UOrbitalBehaviorComponent();

	/** 타겟 중심(앵커) 보간 속도. 0이면 즉시 스냅 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Follow", meta=(ClampMin="0.0"))
	float AnchorLerpSpeed = 3.f;

	/** 등속 추적 사용 시 켜기. 켜면 LerpSpeed 대신 UnitsPerSec가 적용됨 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Follow")
	bool bUseConstantFollow = false;

	/** 등속 추적 속도(cm/s) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Follow", meta=(EditCondition="bUseConstantFollow", ClampMin="0.0"))
	float AnchorFollowUnitsPerSec = 800.f;

	/** 공전 기준 타겟(중심점) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Targets")
	AActor* TargetActor = nullptr;

	/** 바라볼 대상(없으면 회전 고정) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Targets")
	AActor* PlayerActor = nullptr;

	/** 공전 반경 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Orbit", meta=(ClampMin="0.0"))
	float OrbitRadius = 200.f;

	/** 타겟 중심에서의 높이 오프셋 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Orbit")
	float HeightOffset = 0.f;

	/** 공전 각속도(도/초) — 궤도 회전에 사용 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Orbit")
	float IdleYawSpeedDegPerSec = 90.f;

	/** 상하 바운스 진폭 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Vertical")
	float BobAmplitude = 30.f;

	/** 상하 바운스 빈도(Hz) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Vertical", meta=(ClampMin="0.0"))
	float BobSpeed = 1.2f;

	/** 위치 이동 시 충돌 스윕 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Movement")
	bool bSweepMovement = false;

	/** PlayerActor가 유효할 때만 바라봄 (Yaw만 적용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Target Float|Rotation")
	bool bFacePlayer = true;

	/** 타겟 교체 API */
	UFUNCTION(BlueprintCallable, Category="Target Float|Follow")
	void SetTargetActor(AActor* NewTarget, bool bSnapAnchor = false, bool bPreserveOrbitPhase = true);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 내부 상태
	float   AngleDeg  = 0.f;
	float   TimeAcc   = 0.f;
	FVector AnchorLoc = FVector::ZeroVector;

	void Tick_TargetFloat(float DeltaTime);
};
