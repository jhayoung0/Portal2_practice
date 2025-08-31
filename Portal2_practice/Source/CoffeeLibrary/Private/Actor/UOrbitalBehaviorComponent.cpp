// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "Actor/UOrbitalBehaviorComponent.h"
#include "GameFramework/Actor.h"

UOrbitalBehaviorComponent::UOrbitalBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitalBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		AnchorLoc = Owner->GetActorLocation();
	}
}

void UOrbitalBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Tick_TargetFloat(DeltaTime);
}

void UOrbitalBehaviorComponent::Tick_TargetFloat(float DeltaTime)
{
	AActor* Owner = GetOwner();
	if (!Owner || !IsValid(TargetActor) || DeltaTime <= 0.f)
		return;

	// 1) 타겟 중심(높이 오프셋 포함)
	const FVector TargetCenter = TargetActor->GetActorLocation() + FVector(0.f, 0.f, HeightOffset);

	// 2) 앵커 보간(타겟이 바뀌거나 움직여도 부드럽게 따라감)
	AnchorLoc = FMath::VInterpTo(AnchorLoc, TargetCenter, DeltaTime, AnchorLerpSpeed);

	// 3) 시간 누적(상하 바운스 / 공전 각도)
	TimeAcc += DeltaTime;

	// IdleYawSpeedDegPerSec를 공전 각속도로 사용
	AngleDeg = FMath::Fmod(AngleDeg + IdleYawSpeedDegPerSec * DeltaTime, 360.f);
	const float Rad = FMath::DegreesToRadians(AngleDeg);

	// 4) 궤도 오프셋(SinCos 1회 호출)
	float s = 0.f, c = 0.f;
	FMath::SinCos(&s, &c, Rad);

	// 상승/하강 바운스
	const float Bob = FMath::Sin(TimeAcc * (2.f * PI) * FMath::Max(0.f, BobSpeed)) * BobAmplitude;

	const FVector Offset(c * OrbitRadius, s * OrbitRadius, Bob);
	const FVector NewLoc = AnchorLoc + Offset;

	// 5) 위치 적용(충돌 고려가 필요하면 true로 변경)
	Owner->SetActorLocation(NewLoc);

	// 6) 플레이어를 바라보게(수평 Yaw만 적용, 근접 발산 방지)
	if (IsValid(PlayerActor))
	{
		const FVector ToPlayer = PlayerActor->GetActorLocation() - NewLoc;
		if (!ToPlayer.IsNearlyZero(1.f))
		{
			const float DesiredYaw = ToPlayer.Rotation().Yaw;
			Owner->SetActorRotation(FRotator(0.f, DesiredYaw, 0.f));
		}
	}
	else
	{
		// PlayerActor가 없으면 천천히 자체 회전(원한다면 주석 해제)
		const FRotator AddYaw(0.f, IdleYawSpeedDegPerSec * DeltaTime, 0.f);
		Owner->AddActorWorldRotation(AddYaw);
	}
}

void UOrbitalBehaviorComponent::SetTargetActor(AActor* NewTarget, bool bSnapAnchor, bool bPreserveOrbitPhase)
{
	// 동일 대상이면 무시
	if (TargetActor == NewTarget)
		return;

	AActor* Owner = GetOwner();
	const FVector OwnerLoc = (Owner ? Owner->GetActorLocation() : FVector::ZeroVector);

	TargetActor = NewTarget;

	// 새 타겟이 없으면 앵커/각도는 그대로 유지
	if (!IsValid(TargetActor))
		return;

	// 새 중심(높이 오프셋 반영)
	const FVector NewCenter = TargetActor->GetActorLocation() + FVector(0.f, 0.f, HeightOffset);

	// 즉시 스냅(보간 없이 전환)
	if (bSnapAnchor)
	{
		AnchorLoc = NewCenter;
	}

	// 위상 유지: 현재 월드 위치가 궤도상 같은 지점이 되도록 각도를 재설정
	if (bPreserveOrbitPhase && Owner)
	{
		const FVector Rel = OwnerLoc - NewCenter; // 새 중심 기준
		if (!Rel.IsNearlyZero())
		{
			AngleDeg = FMath::RadiansToDegrees(FMath::Atan2(Rel.Y, Rel.X)); // [-180,180]
			// AngleDeg를 0~360로 정규화하고 싶으면:
			if (AngleDeg < 0.f) AngleDeg += 360.f;
		}
	}

	// TimeAcc는 유지(바운스 위상 지속). 필요 시 리셋:
	// TimeAcc = 0.f;
}