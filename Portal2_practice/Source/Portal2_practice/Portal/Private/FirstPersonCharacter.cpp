// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonCharacter.h"
#include "PortalActor.h"
#include "WeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputAction.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"


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

	//Arrow 설정
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(FirstPersonCamera);
	ArrowComp->SetRelativeLocation(FVector(0.f,0.f,-30.f));
	
	
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LP = PC->GetLocalPlayer())
		{
			class UEnhancedInputLocalPlayerSubsystem* Inputsys;
			
			Inputsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LP);
			if (Inputsys)
			{
				Inputsys->AddMappingContext(IMC_Shoot, 0);
				Inputsys->AddMappingContext(IMC_MouseLook, 1);
				
			}
		}
	}

	
	
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

	



void AFirstPersonCharacter::DestroyPortal(TSubclassOf<APortalActor> Portal)
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy Portal"));
	APortalActor* FoundedPortal = Cast<APortalActor>
	(UGameplayStatics::GetActorOfClass(GetWorld(), Portal));

	if (FoundedPortal)
	{FoundedPortal->Destroy();}
}

void AFirstPersonCharacter::SetPortalLocAndRot(float forward_float, float comparison_value,
	FRotator PortalRotation, FRotator Rot, FVector PortalLocation, FVector Offset)
{
	const float T = 0.01f;
	bool IsNearyEqual;
	
	if (FMath::IsNearlyEqual(forward_float, comparison_value, T))
	{
		IsNearyEqual = true;
	}
	else
	{
		IsNearyEqual = false;		
	}

	// 포탈 전방 벡터에 따라 회전과 위치 설정
	if (IsNearyEqual)
	{
		PortalRotation = Rot;
		PortalLocation += Offset;
	}
	
}


// 포탈 스폰 함수
void AFirstPersonCharacter::SpawnPortal(TSubclassOf<APortalActor> PortalClass,FVector PortalLoc, FVector PortalForwardVector, FRotator PortalRot)
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Portal"));
	if (WeaponCP->CanShoot)
	{

		//총 소리 재생
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),bulletsound,FVector(0,0,0),
			FRotator(0,0,0));

		FirstPersonMesh->GetAnimInstance()->Montage_Play(Pistol_fire_montage);

		FVector endloc = ArrowComp->GetComponentLocation() + ArrowComp->GetForwardVector()*5000.f;

		FHitResult Hit;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel3));//Wall
		
		UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld()
			,ArrowComp->GetComponentLocation()
			,endloc
			,ObjectTypes
			,false
			,TArray<AActor*>()
			,EDrawDebugTrace::Type::None
			,Hit
			,true);

		
		PortalLoc = Hit.ImpactPoint;
		PortalForwardVector = Hit.ImpactNormal;
	
		//포탈 위치 및 회전 설정4

		//X
		SetPortalLocAndRot(PortalForwardVector.X,1.f,
		PortalRot,FRotator(0,0,0),
		PortalLoc, FVector(1,0,0));

		SetPortalLocAndRot(PortalForwardVector.X,-1.f,
		PortalRot,FRotator(0,0,-180),
		PortalLoc, FVector(-1,0,0));

		
		// Y
		SetPortalLocAndRot(PortalForwardVector.Y,1.f,
		PortalRot,FRotator(0,0,90),
		PortalLoc, FVector(0,1,0));

		SetPortalLocAndRot(PortalForwardVector.Y,-1.f,
		PortalRot,FRotator(0,0,-90),
		PortalLoc, FVector(0,-1,0));

		
		//Z
		SetPortalLocAndRot(PortalForwardVector.Z,1.f,
		PortalRot,FRotator(0,90,0),
		PortalLoc, FVector(0,0,1));

		SetPortalLocAndRot(PortalForwardVector.Z,-1.f,
		PortalRot,FRotator(0,-90,0),
		PortalLoc, FVector(0,0,-1));
		
		// 기존 포탈 삭제
		DestroyPortal(PortalClass);
		
		// 새로운 포탈 스폰
		PortalActor = GetWorld()->SpawnActor<APortalActor>(PortalClass, PortalLoc, PortalRot);
	 
		// 크기 애니메이션 넣기 (나중에..)
		
		
		
	}
}