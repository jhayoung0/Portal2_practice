// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskSyncManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class PORTAL2_PRACTICE_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;


	
	// 변수 선언
	// FirstPersonCharacter.h (플레이어에 보관할 포인터는 UPROPERTY로!)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	UWeaponComponent* WeaponCP = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanShoot;

	// 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPrimitiveComponent* GrabbedObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AFirstPersonCharacter* player;



	
};
