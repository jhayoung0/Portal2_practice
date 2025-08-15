// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "UEaseComponent.h"

UEaseComponent::UEaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	this->UpdateTrack(DeltaTime);
}

void UEaseComponent::UpdateTrack(float DeltaTime)
{
	for (auto& Elem : FloatTracks)
	{
		Elem.Value.Update(DeltaTime);
	}

	for (auto& Elem : VectorTracks)
	{
		Elem.Value.Update(DeltaTime);
	}

	for (auto& Elem : RotatorTracks)
	{
		Elem.Value.Update(DeltaTime);
	}
}

float UEaseComponent::GetTrackAlpha(FName TrackName) const
{
	if (auto* ElemPtr = FloatTracks.Find(TrackName))
	{
		return ElemPtr->GetAlpha();
	}

	if (auto* ElemPtr = VectorTracks.Find(TrackName))
	{
		return ElemPtr->GetAlpha();
	}

	if (auto* ElemPtr = RotatorTracks.Find(TrackName))
	{
		return ElemPtr->GetAlpha();
	}

	return 0.f;
}

float UEaseComponent::GetEaseFloatTrack(FName TrackName)
{
	return FloatTracks.Contains(TrackName) ? FloatTracks[TrackName].Current : 0.f;
}

void UEaseComponent::SetEaseFloatTrack(FName TrackName, EEaseType EaseType, float Start, float Target, float Duration)
{
	FEaseFloatTrack& Track = FloatTracks.FindOrAdd(TrackName);
	Track.EaseType = EaseType;
	Track.Start = Start;
	Track.Target = Target;
	Track.Duration = Duration;
	Track.ElapsedTime = 0.f;
}

FVector UEaseComponent::GetEaseVectorTrack(FName TrackName)
{
	return VectorTracks.Contains(TrackName) ? VectorTracks[TrackName].Current : FVector::ZeroVector;
}

void UEaseComponent::SetEaseVectorTrack(FName TrackName, EEaseType EaseType, FVector Start, FVector Target, float Duration)
{
	FEaseVectorTrack& Track = VectorTracks.FindOrAdd(TrackName);
	Track.EaseType = EaseType;
	Track.Start = Start;
	Track.Target = Target;
	Track.Duration = Duration;
	Track.ElapsedTime = 0.f;
}

FRotator UEaseComponent::GetEaseRotatorTrack(FName TrackName)
{
	return RotatorTracks.Contains(TrackName) ? RotatorTracks[TrackName].Current : FRotator::ZeroRotator;
}

void UEaseComponent::SetEaseRotatorTrack(FName TrackName, EEaseType EaseType, FRotator Start, FRotator Target, float Duration)
{
	FEaseRotatorTrack& Track = RotatorTracks.FindOrAdd(TrackName);
	Track.EaseType = EaseType;
	Track.Start = Start;
	Track.Target = Target;
	Track.Duration = Duration;
	Track.ElapsedTime = 0.f;
}