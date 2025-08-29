// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskSyncManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "WeaponComponent.generated.h"


UCLASS()
class PORTAL2_PRACTICE_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	UWeaponComponent(); 

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	
	// 변수 선언
	// FirstPersonCharacter.h (플레이어에 보관할 포인터는 UPROPERTY로!)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	UWeaponComponent* WeaponCP = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanShoot;

	// 변수 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPrimitiveComponent* GrabbedObject;

	UPROPERTY(EditAnywhere)
	class AFirstPersonCharacter* player;

	
	// 인풋 변수
	UPROPERTY(EditAnywhere)
	APlayerController* PC; 

	UPROPERTY(EditAnywhere)
	class APawn* OwnerPawn;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_ShootL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_ShootR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* IA_Grab;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PortalBulletBlue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PortalBulletOrange;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APortalActor> Portal1class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class APortalActor> Portal2class;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* VFX_Blue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem*  VFX_Orange;
	

	bool bInputBound = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	APlayerCameraManager* CamManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AmmoSocket_loc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator AmmoSocket_Rot;

	
	//Function
	UFUNCTION()
	void OnShootL(const FInputActionValue& value);

	UFUNCTION()
	void OnShootR(const FInputActionValue& value);
	
	UFUNCTION()
	void OnGrab(const FInputActionValue& value);

	UFUNCTION()
	void OnLinetrace(bool color);
	
	UFUNCTION(BlueprintPure)
	AActor* SpawnBullet(bool color);

	UFUNCTION()
	void FlyBullet(FVector start, bool color,
		FVector EndImpactPoint, FVector EndBullet);
	

	UFUNCTION()
	void SetupInput(UEnhancedInputComponent* EIC);
	

	// 점프대 올라가면 GRAB 자동 해제
	UFUNCTION(BlueprintCallable)
	void ReleaseObjectCube();
};
