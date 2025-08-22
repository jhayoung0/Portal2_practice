// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalCube.h"

#include "Features/UObjectPoolManager.h"
#include "Shared/FComponentHelper.h"

#define SYMBOL_PATH			TEXT("StaticMesh_Symbol")


APortalCube::APortalCube()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APortalCube::BeginPlay()
{
	Super::BeginPlay();

	SymbolMesh = FComponentHelper::FindComponentByNameRecursive<UStaticMeshComponent>(this, SYMBOL_PATH);

	if( SymbolMesh != nullptr)
	{
		SymbolMaterial = SymbolMesh->CreateDynamicMaterialInstance(0);
		SymbolMaterial->SetVectorParameterValue( ColorParam, DeactivateColor );
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
