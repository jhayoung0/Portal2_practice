// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APickupRifle.generated.h"





UCLASS()
class PORTAL2_PRACTICE_API APickupRifle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	// skeletalmesh, sphere 붙이기

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* SceneComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalMeshComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WdgAimClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PortalGunStandClass;
	
	UFUNCTION()
	void OnSphereCompOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
