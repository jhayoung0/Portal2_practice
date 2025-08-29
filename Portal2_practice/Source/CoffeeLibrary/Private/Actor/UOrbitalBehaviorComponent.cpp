// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "Actor/UOrbitalBehaviorComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UOrbitalBehaviorComponent::UOrbitalBehaviorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitalBehaviorComponent::BeginPlay()
{
    Super::BeginPlay();
    AnchorLoc = GetOwner()->GetActorLocation();
}

void UOrbitalBehaviorComponent::SetState(EOrbitalState NewState)
{
    State = NewState;

    // 상태 전환 초기화
    TimeAcc = 0.f;
    AscendTravel = 0.f;
    AngleDeg = 0.f;

    if (State == EOrbitalState::TargetFloat)
    {
        AnchorLoc = TargetActor ? TargetActor->GetActorLocation()
                                : GetOwner()->GetActorLocation();
    }
    else if (State == EOrbitalState::AscendAway)
    {
        AnchorLoc = GetOwner()->GetActorLocation();
    }
}

void UOrbitalBehaviorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (State)
    {
    case EOrbitalState::PlayerFollow:
        Tick_PlayerFollow(DeltaTime);
        break;
        
    case EOrbitalState::TargetFloat:
        Tick_TargetFloat(DeltaTime);
        break;
        
    case EOrbitalState::AscendAway:
        Tick_AscendAway(DeltaTime);
        break;

    default:
        break;
    }
}

void UOrbitalBehaviorComponent::Tick_PlayerFollow(float DT)
{
    AActor* Owner = GetOwner();

    if (!Owner || !IsValid(PlayerActor))
        return;

    const FVector Center = PlayerActor->GetActorLocation() + FVector(0,0,HeightOffset);
    AngleDeg = FMath::Fmod(AngleDeg + OrbitSpeedDegPerSec * DT, 360.f);

    const float Rad = FMath::DegreesToRadians(AngleDeg);
    const FVector Offset(FMath::Cos(Rad)*OrbitRadius, FMath::Sin(Rad)*OrbitRadius, 0.f);

    const FVector NewLoc = Center + Offset;
    Owner->SetActorLocation(NewLoc);

    // 항상 플레이어를 바라봄
    const FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(NewLoc, PlayerActor->GetActorLocation());
    Owner->SetActorRotation(LookAt);
}

void UOrbitalBehaviorComponent::Tick_TargetFloat(float DeltaTime)
{
    AActor* Owner = GetOwner();
    if (!Owner)
        return;

    if (IsValid(TargetActor))
        AnchorLoc = TargetActor->GetActorLocation(); // 타겟이 움직이면 앵커도 갱신

    TimeAcc += DeltaTime;
    const float Bob = FMath::Sin(TimeAcc * BobSpeed) * BobAmplitude;

    FVector NewLoc = AnchorLoc;
    NewLoc.Z += Bob;
    Owner->SetActorLocation(NewLoc);

    // 천천히 회전
    const FRotator AddYaw(0.f, IdleYawSpeedDegPerSec * DeltaTime, 0.f);
    Owner->AddActorWorldRotation(AddYaw);
}

void UOrbitalBehaviorComponent::Tick_AscendAway(float DeltaTime)
{
    AActor* Owner = GetOwner();
    if (!Owner)
        return;

    AscendTravel += AscendSpeed * DeltaTime;

    FVector NewLoc = AnchorLoc;
    NewLoc.Z += AscendTravel;
    Owner->SetActorLocation(NewLoc);

    if (AscendTravel >= AscendDistance)
    {
        if (bDestroyOnVanish)
        {
            Owner->Destroy();
        }
        else
        {
            Owner->SetActorHiddenInGame(true);
            SetComponentTickEnabled(false);
        }
    }
}
