// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "PortalCube.h"
#include "ULog.h"
#include "Features/UObjectPoolManager.h"
#include "Shared/FComponentHelper.h"

#define BODY_PATH			TEXT("StaticMesh_Body")
#define COVER_PATH			TEXT("StaticMesh_Cover")
#define SYMBOL_PATH			TEXT("StaticMesh_Symbol")

APortalCube::APortalCube()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APortalCube::BeginPlay()
{
	Super::BeginPlay();

	BodyMesh = FComponentHelper::FindComponentByNameRecursive<UStaticMeshComponent>(this, BODY_PATH);
	if( IsValid(BodyMesh ))
	{
		BodyMaterial = BodyMesh->CreateDynamicMaterialInstance(0);
		BodyMaterial->SetScalarParameterValue( DisolveParam, DissolveBegin );
	}

	CoverMesh = FComponentHelper::FindComponentByNameRecursive<UStaticMeshComponent>(this, COVER_PATH);
	if( IsValid(CoverMesh ))
	{
		CoverMaterial = CoverMesh->CreateDynamicMaterialInstance(0);
		CoverMaterial->SetScalarParameterValue( DisolveParam, DissolveBegin );
	}
	
	SymbolMesh = FComponentHelper::FindComponentByNameRecursive<UStaticMeshComponent>(this, SYMBOL_PATH);
	if( IsValid(SymbolMesh) )
	{
		SymbolMaterial = SymbolMesh->CreateDynamicMaterialInstance(0);
		SymbolMaterial->SetVectorParameterValue( ColorParam, DeactivateColor );
		SymbolMaterial->SetScalarParameterValue( DisolveParam, DissolveBegin );
	}
}

void APortalCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APortalCube::ActivateCube_Implementation()
{
	SymbolMaterial->SetVectorParameterValue( ColorParam, ActivateColor );
}

void APortalCube::DeactivateCube_Implementation()
{
	SymbolMaterial->SetVectorParameterValue( ColorParam, DeactivateColor );
}

void APortalCube::ReturnPool_Implementation()
{
	auto PoolManager = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
	if (PoolManager != nullptr )
		PoolManager->ReturnActorToPool(this);
}

void APortalCube::StartDisolve_Implementation()
{
	DissolveElapsed = 0.0f;

	GetWorldTimerManager().SetTimer(DissolveTimerHandle,
		this,
		&APortalCube::UpdateDissolve,
		0.02f,
		true);

	this->ActivateDisolve();
}

void APortalCube::ActivateDisolve_Implementation()
{
}

void APortalCube::UpdateDissolve()
{
	DissolveElapsed +=  0.02f;

	const float t = FMath::Clamp(DissolveElapsed / DissolveDuration, 0.f, 1.f);
	const float Alpha = FMath::Lerp(DissolveBegin, DissolveEnd, t);

	if (IsValid(BodyMaterial))   BodyMaterial->SetScalarParameterValue(DisolveParam, Alpha);
	if (IsValid(CoverMaterial))  CoverMaterial->SetScalarParameterValue(DisolveParam, Alpha);
	if (IsValid(SymbolMaterial)) SymbolMaterial->SetScalarParameterValue(DisolveParam, Alpha);

	if (t >= 1.f)
	{
		// 타이머 종료
		GetWorldTimerManager().ClearTimer(DissolveTimerHandle);

		if (IsValid(BodyMaterial))   BodyMaterial->SetScalarParameterValue(DisolveParam, DissolveBegin);
		if (IsValid(CoverMaterial))  CoverMaterial->SetScalarParameterValue(DisolveParam, DissolveBegin);
		if (IsValid(SymbolMaterial)) SymbolMaterial->SetScalarParameterValue(DisolveParam, DissolveBegin);
		
		auto PoolManager = GetGameInstance()->GetSubsystem<UObjectPoolManager>();
		if (PoolManager != nullptr )
			PoolManager->ReturnActorToPool(this);
	}
}


// 큐브 포탈 이동
void APortalCube::Activate_Duplicate(USceneComponent* EntryDoorComponent, USceneComponent* ExitDoorComponent)
{
}

void APortalCube::Deactivate_Duplicate()
{
}