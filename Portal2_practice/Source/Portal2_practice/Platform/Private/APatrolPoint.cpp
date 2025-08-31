// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "APatrolPoint.h"


APatrolPoint::APatrolPoint()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APatrolPoint::BeginPlay()
{
	Super::BeginPlay();
}

void APatrolPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}