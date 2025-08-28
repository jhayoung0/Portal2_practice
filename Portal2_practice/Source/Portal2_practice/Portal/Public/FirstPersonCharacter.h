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
	
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TSubclassOf<class UWeaponComponent> WeaponCPClass;  // 설계도(클래스) 선택

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	class UWeaponComponent* WeaponCP = nullptr;         // 실제 인스턴스


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	bool HasRifle;    
	

	
};
