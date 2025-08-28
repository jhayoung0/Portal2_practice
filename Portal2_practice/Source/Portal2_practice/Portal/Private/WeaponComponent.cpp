// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "FirstPersonCharacter.h"

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();


	if (AActor* Owner = GetOwner())
	{
		player = Cast<AFirstPersonCharacter>(Owner);
	}

	AttachToComponent(player->FirstPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale,
	 TEXT("GripPoint"));

	//player의 has rifle을 true로 set
	player->HasRifle = true;
	
}

void UWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	
}
