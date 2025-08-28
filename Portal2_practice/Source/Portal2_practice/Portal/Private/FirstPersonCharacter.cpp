// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 설정
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(30.f, 0.f,60.f));
	FirstPersonCamera->bEditableWhenInherited = true;

	
	// firstperson mesh 설정
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonMesh->SetRelativeLocation(FVector(-22.f, -5.f, -148.f));
	FirstPersonMesh->bEditableWhenInherited = true;
	
	/* HL, HR
	HL = CreateDefaultSubobject<USceneComponent>(TEXT("HL"));
	HL->SetupAttachment(FirstPersonMesh);
	HL->SetRelativeLocation(FVector(-35.f, 3.f, 11.f));
	HL->bEditableWhenInherited = true;

	
	HR = CreateDefaultSubobject<USceneComponent>(TEXT("HR"));
	HR->SetupAttachment(FirstPersonMesh);
	HR->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	HR->bEditableWhenInherited = true;
	*/

	

}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

