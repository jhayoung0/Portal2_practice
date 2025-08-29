// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPersonCharacter.generated.h"

UCLASS()
class PORTAL2_PRACTICE_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* FirstPersonCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* FirstPersonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* ArrowComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<class UWeaponComponent> WeaponCPClass;  // 설계도(클래스) 선택

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	class UWeaponComponent* WeaponCP = nullptr;         // 실제 인스턴스


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	bool HasRifle;    
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal")
	bool NewlyCreatedPortal1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal")
	bool NewlyCreatedPortal2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Portal1Class;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Portal2Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal")
	FVector PortalLoc;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal")
	FVector PortalForwardVector;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Portal")
	FRotator PortalRot;
	
	UPROPERTY(EditAnywhere, Category="Sound")
	USoundBase* bulletsound;
	
	UPROPERTY(EditAnywhere, Category = "Anim")
	UAnimMontage* Pistol_fire_montage;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APortalActor* PortalActor;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UInputMappingContext* IMC_Shoot; 


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* IMC_MouseLook;
	
	// function

	
	UFUNCTION(BlueprintCallable, Category="Portal")
	void SpawnPortal(TSubclassOf<APortalActor> PortalClass); // , FVector PortalLoc, FVector PortalForwardVector, FRotator PortalRot);
	

	UFUNCTION(BlueprintCallable, Category="Portal")
	void SetPortalLocAndRot(float forward_float, float comparison_value
		,FRotator PortalRotation, FRotator Rot, FVector PortalLocation, FVector Offset);


	
	UFUNCTION(BlueprintCallable, Category="Portal")
	void DestroyPortal(TSubclassOf<APortalActor> Portal);
	 
	
	
};
