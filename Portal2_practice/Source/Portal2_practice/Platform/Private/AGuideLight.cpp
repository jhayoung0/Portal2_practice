// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "AGuideLight.h"
#include "Shared/FComponentHelper.h"

#define GUIDELIGHT_COLLISION_PATH		TEXT("Light")

AGuideLight::AGuideLight()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGuideLight::BeginPlay()
{
	Super::BeginPlay();

	auto LightComp = FComponentHelper::FindComponentByNameRecursive<UStaticMeshComponent>(this, GUIDELIGHT_COLLISION_PATH);
	LightMaterial = LightComp->CreateDynamicMaterialInstance(0);

	this->UpdateLight();
}

void AGuideLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}