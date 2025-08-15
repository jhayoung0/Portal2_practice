#include "UParabolaComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

UParabolaComponent::UParabolaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UParabolaComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& P : BallisticTracks)
		P.Value.ResetTime();
	for (auto& P : GeometricTracks)
		P.Value.ResetTime();
}

void UParabolaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateTracks(DeltaTime);
}

void UParabolaComponent::UpdateTracks(float DeltaTime)
{
	for (auto& P : BallisticTracks)
		P.Value.Advance(DeltaTime);
	for (auto& P : GeometricTracks)
		P.Value.Advance(DeltaTime);
}



FRotator UParabolaComponent::GetParabolaFacing( FName TrackName, bool bYawOnly, EForwardAxis ForwardAxis ) const
{
	if (const FParabolaBallisticTrack* Ballistic = BallisticTracks.Find(TrackName))
	{
		const float CurAlpha  = FMath::Clamp(Ballistic->GetAlpha(), 0.f, 1.f);
		const float NextAlpha = FMath::Clamp(CurAlpha + 0.08f, 0.f, 1.f);

		if (FMath::IsNearlyEqual(CurAlpha, NextAlpha))
			return GetOwner()->GetActorRotation();

		const FVector CurLocation  = Ballistic->EvaluateAtAlpha(GetOwner(), CurAlpha);
		const FVector NextLocation = Ballistic->EvaluateAtAlpha(GetOwner(), NextAlpha);

		return MakeFacingFromDir(NextLocation - CurLocation, bYawOnly, ForwardAxis);
	}
	else if (const FParabolaGeometricTrack* Geometric = GeometricTracks.Find(TrackName))
	{
		const float CurAlpha  = FMath::Clamp(Geometric->GetAlpha(), 0.f, 1.f);
		const float NextAlpha = FMath::Clamp(CurAlpha + 0.08f, 0.f, 1.f);

		if (FMath::IsNearlyEqual(CurAlpha, NextAlpha))
			return GetOwner()->GetActorRotation();

		const FVector CurLocation  = Geometric->EvaluateAtAlpha(GetOwner(), CurAlpha);
		const FVector NextLocation = Geometric->EvaluateAtAlpha(GetOwner(), NextAlpha);

		return MakeFacingFromDir(NextLocation - CurLocation, bYawOnly, ForwardAxis);
	}

	return GetOwner()->GetActorRotation();
}

FRotator UParabolaComponent::MakeFacingFromDir(const FVector& Direction, const bool bYawOnly, const EForwardAxis ForwardAxis) const
{
	FVector Dir = Direction;

	if (bYawOnly)
		Dir.Z = 0.f;

	if (!Dir.Normalize())
		return GetOwner() ? GetOwner()->GetActorRotation() : FRotator::ZeroRotator;

	FRotator Rot;
	switch (ForwardAxis)
	{
	case EForwardAxis::X: Rot = FRotationMatrix::MakeFromX(Dir).Rotator(); break;
	case EForwardAxis::Y: Rot = FRotationMatrix::MakeFromY(Dir).Rotator(); break;
	case EForwardAxis::Z: Rot = FRotationMatrix::MakeFromZ(Dir).Rotator(); break;
	default:              Rot = FRotationMatrix::MakeFromX(Dir).Rotator(); break;
	}

	if (bYawOnly)
	{
		Rot.Pitch = 0.f;
		Rot.Roll  = 0.f;
	}
	else
	{
		Rot.Roll = 0.f;
	}

	return Rot;
}

// ---- Ballistic ----
void UParabolaComponent::SetBallisticParabolaTrack(FName TrackName, const FParabolaBallisticTrack& Track)
{
	FParabolaBallisticTrack Copy = Track;
	Copy.ResetTime();
	BallisticTracks.FindOrAdd(TrackName) = Copy;
}

FVector UParabolaComponent::GetBallisticParabolaVectorTrack(FName TrackName) const
{
	if (const FParabolaBallisticTrack* T = BallisticTracks.Find(TrackName))
	{
		return T->EvaluateAtCurrent(GetOwner());
	}
	return FVector::ZeroVector;
}

FVector UParabolaComponent::GetBallisticVectorAtAlphaFromTrack(FName TrackName, float Alpha) const
{
	if (const FParabolaBallisticTrack* T = BallisticTracks.Find(TrackName))
	{
		return T->EvaluateAtAlpha(GetOwner(), Alpha);
	}
	return FVector::ZeroVector;
}

void UParabolaComponent::DrawBallisticPath(FName TrackName, int32 NumSegments, FColor Color, float LifeTime) const
{
	if (const FParabolaBallisticTrack* T = BallisticTracks.Find(TrackName))
	{
		const float StepTime = T->Duration / NumSegments;
		FVector PrevPos = T->EvaluateAtTime(GetOwner(), 0.f);

		for (int32 i = 1; i <= NumSegments; ++i)
		{
			float t = StepTime * i;
			FVector CurrPos = T->EvaluateAtTime(GetOwner(), t);

			DrawDebugLine(
				GetWorld(),
				PrevPos,
				CurrPos,
				Color,
				false,   // 지속 여부
				LifeTime,    // 지속 시간 (초)
				0,       // Depth Priority
				1.5f     // 두께
			);

			PrevPos = CurrPos;
		}
	}
}



// ---- Geometric ----
void UParabolaComponent::SetGeometricParabolaTrack(FName TrackName, const FParabolaGeometricTrack& Track)
{
	FParabolaGeometricTrack Copy = Track;
	Copy.ResetTime();
	GeometricTracks.FindOrAdd(TrackName) = Copy;
}

FVector UParabolaComponent::GetGeometricParabolaVectorTrack(FName TrackName) const
{
	if (const FParabolaGeometricTrack* T = GeometricTracks.Find(TrackName))
	{
		return T->EvaluateAtCurrent(GetOwner());
	}
	return FVector::ZeroVector;
}

FVector UParabolaComponent::GetGeometricVectorAtAlphaFromTrack(FName TrackName, float Alpha) const
{
	if (const FParabolaGeometricTrack* T = GeometricTracks.Find(TrackName))
	{
		return T->EvaluateAtAlpha(GetOwner(), Alpha);
	}
	return FVector::ZeroVector;
}

void UParabolaComponent::DrawGeometricPath(FName TrackName, int32 NumSegments, FColor Color, float LifeTime ) const
{
	if (const FParabolaGeometricTrack* T = GeometricTracks.Find(TrackName))
	{
		const float StepAlpha = 1.f / NumSegments;
		FVector PrevPos = T->EvaluateAtAlpha(GetOwner(), 0.f);

		for (int32 i = 1; i <= NumSegments; ++i)
		{
			float a = StepAlpha * i;
			FVector CurrPos = T->EvaluateAtAlpha(GetOwner(), a);

			DrawDebugLine(
				GetWorld(),
				PrevPos,
				CurrPos,
				Color,
				false,
				LifeTime,
				0,
				1.5f
			);

			PrevPos = CurrPos;
		}
	}
}