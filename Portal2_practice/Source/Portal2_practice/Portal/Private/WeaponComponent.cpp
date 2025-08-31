// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "FirstPersonCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "InputActionValue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemlibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "PortalCube.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


UWeaponComponent::UWeaponComponent()
{
	// Tick 켜주기
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());
	PC = Cast<APlayerController>(OwnerPawn->GetController());
	CamManager = PC->PlayerCameraManager;
	
	// 포탈건 붙이기
	if (AActor* Owner = GetOwner())
	{
		player = Cast<AFirstPersonCharacter>(Owner);
	}

	AttachToComponent(player->FirstPersonMesh, FAttachmentTransformRules::SnapToTargetIncludingScale,
	 TEXT("GripPoint"));

	//player의 has rifle을 true로 set
	player->HasRifle = true;
	

}


//Event tick
void UWeaponComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}


//input action
void UWeaponComponent::OnShootL(const FInputActionValue& value)
{

	UE_LOG(LogTemp, Warning, TEXT("ShootL"));

	player->NewlyCreatedPortal1 = false;
	player->SpawnPortal(Portal1class, true);


	if (CamManager)
	{
		OnLinetrace(true);
	}
	
	

}

void UWeaponComponent::OnShootR(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("ShootR"));
	
	player->NewlyCreatedPortal2 = false;
	player->SpawnPortal(Portal2class, false);

	
	if (CamManager)
	{
		OnLinetrace(false);
	}


	
	
}

void UWeaponComponent::OnGrab(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
	AmmoSocket_loc  = GetSocketLocation(FName("AmmoSocket"));
	

	FVector Offset  = FVector(100.f, -20.f, -10.f);
	FRotator CamRot = CamManager->GetCameraRotation();
	
	FVector start = AmmoSocket_loc + UKismetMathLibrary::GreaterGreater_VectorRotator(Offset, CamRot);
	
	if (IsValid(GrabbedObject))
	{

		
	}
	else
	{
		// LineTrace
		FHitResult Hit;

		FVector grabend = AmmoSocket_loc +
			UKismetMathLibrary::GetForwardVector(AmmoSocket_Rot) * 500.f;
		
		bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			AmmoSocket_loc,
			grabend,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			TArray<AActor*>(),        // 비움
			EDrawDebugTrace::None,    
			Hit,
			true                      // Ignore Self
		);

		if (bHit){
			if (Hit.Component->IsSimulatingPhysics())
			{
				Hit.Component->SetSimulatePhysics(false);
				//AttachToComponent(this, Hit.GetActor(),
					//FAttachmentTransformRules::KeepWorldTransform, FName("AmmoSocket"));
					
				
				
//(USceneComponent* Parent, FName SocketName, EAttachmentRule LocationRule, EAttachmentRule RotationRule, EAttachmentRule ScaleRule, bool bWeldSimulatedBodies);

			}
			
			
			
		}
		
	}

	
}

void UWeaponComponent::OnLinetrace(bool color)
{
	AmmoSocket_loc  = GetSocketLocation(FName("AmmoSocket"));
	
	FVector end = AmmoSocket_loc + (CamManager->GetActorForwardVector() * 500.f);

	// LineTrace
	FHitResult Hit;
		
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		AmmoSocket_loc,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		TArray<AActor*>(),        // 비움
		EDrawDebugTrace::None,    
		Hit,
		true                      // Ignore Self
	);

	// 총알 날아가는거 설정
	FlyBullet(Hit.TraceStart, color, Hit.ImpactPoint, Hit.TraceEnd);

	//UE_LOG(LogTemp, Warning, TEXT("%d", ImpactPoint));
	// 파티클 스폰 (포탈 스폰되지 않더라도 생기게)
	UParticleSystem* particle = color? VFX_Blue :  VFX_Orange;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particle,  Hit.TraceEnd);
	
	
}

AActor* UWeaponComponent::SpawnBullet(bool color)
{
	
	TSubclassOf<AActor> BulletClass = color ? PortalBulletBlue : PortalBulletOrange;

	
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(BulletClass,FVector(0), FRotator(0));
	
	return Spawned;
	
}

void UWeaponComponent::FlyBullet(FVector start, bool color, FVector EndImpactPoint, FVector EndBullet)
// color는 true면 orange, false면 blue임.
{
	AActor* bullet = SpawnBullet(color);
	bullet->SetActorLocation(start);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget     = this;                     // 완료 함수가 있는 객체// 완료 후 호출할 함수 이름
	LatentInfo.Linkage            = 0;                        // 보통 0
	LatentInfo.UUID               = __LINE__;                 // 고유 ID (간단히 라인 넘버 쓰기도 함)
	
	UKismetSystemLibrary::MoveComponentTo(bullet->GetRootComponent(), EndBullet,
		GetComponentRotation(), false, false, 0.2f,
		false, EMoveComponentAction::Move, LatentInfo);

	bullet->SetLifeSpan(0.22f);

}


void UWeaponComponent::SetupInput(UEnhancedInputComponent* EIC)
{

	if (!EIC) { UE_LOG(LogTemp, Error, TEXT("EIC is NULL")); return; }
	if (!IA_ShootL) { UE_LOG(LogTemp, Error, TEXT("IA_Shoot is NULL (assign it in the WeaponComponent)")); return; }

	

	if (IA_ShootL)
	{
		EIC->BindAction(IA_ShootL, ETriggerEvent::Triggered, this, &UWeaponComponent::OnShootL);
	}
	if (IA_ShootR)
	{
		EIC->BindAction(IA_ShootR, ETriggerEvent::Triggered, this, &UWeaponComponent::OnShootR);
	}
	if (IA_Grab)
	{
		EIC->BindAction(IA_Grab, ETriggerEvent::Started, this, &UWeaponComponent::OnGrab);
	}
	
}


void UWeaponComponent::ReleaseObjectCube()
{

	/*
	if (IsValid(GrabbedObject))
	{
		
		Cast<APortalCube>(GrabbedObject->GetOwner())
		->Deactivate_Duplicate();


		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;                   // 딜레이 끝난 뒤 실행될 객체
		LatentInfo.ExecutionFunction = FName("OnDelayDone");// 실행할 함수 이름
		LatentInfo.Linkage = 0;
		LatentInfo.UUID = __LINE__; 

		UKismetSystemLibrary::Delay(this, 0.2f, LatentInfo);

		if (IsValid(GrabbedObject))
		{
			GrabbedObject->SetSimulatePhysics(true);

			GrabbedObject = nullptr;
			
			CanShoot = true; 

		}
		
		
	}
	*/
}



