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
#include "Kismet/KismetSystemlibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "AimUI.h"
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
	player->SpawnPortal(Portal1class,  player->Portal1Loc
		, player->Portal1Forward, player->Portal1Rot);
	
	Orangecore_DynMat = CreateAndSetMaterialInstanceDynamicFromMaterial(1, Bluecore);

	if (!CamManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("cammanager none"));
	}
	if (CamManager)
	{
		OnLinetrace(true);
	}

	// (1) WDGAim 찾기
	TArray<UUserWidget*> Found;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Found, UAimUI::StaticClass(), /*TopLevelOnly*/ true);

	// (2) 첫 번째(인덱스 0)만 사용
	if (Found.Num() > 0)
	{
		if (UAimUI* Aim = Cast<UAimUI>(Found[0]))
		{
			Aim->OnBlueFired();  
		}
	}
	

}

void UWeaponComponent::OnShootR(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("ShootR"));
	
	player->NewlyCreatedPortal2 = false;
	player->SpawnPortal(Portal2class, player->Portal2Loc
		, player->Portal2Forward, player->Portal2Rot);
	
	Bluecore_DynMat = CreateAndSetMaterialInstanceDynamicFromMaterial(1, Orangecore);

	
	if (CamManager)
	{
		OnLinetrace(false);
	}

	// (1) WDGAim 찾기
	TArray<UUserWidget*> Found;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Found, UAimUI::StaticClass(), /*TopLevelOnly*/ true);

	// (2) 첫 번째(인덱스 0)만 사용
	if (Found.Num() > 0)
	{
		if (UAimUI* Aim = Cast<UAimUI>(Found[0]))
		{
			Aim->OnOrangeFired();  
		}
	}
	
	
}

void UWeaponComponent::OnGrab(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
}

void UWeaponComponent::OnLinetrace(bool color)
{
	FVector AmmoSocket_loc = GetSocketLocation(FName("AmmoSocket"));
	
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
	AActor* Bullet = SpawnBullet(color);
	Bullet->SetActorLocation(start);

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget     = this;                     // 완료 함수가 있는 객체
	LatentInfo.ExecutionFunction  = FName("OnMoveFinished");  // 완료 후 호출할 함수 이름
	LatentInfo.Linkage            = 0;                        // 보통 0
	LatentInfo.UUID               = __LINE__;                 // 고유 ID (간단히 라인 넘버 쓰기도 함)
	
	UKismetSystemLibrary::MoveComponentTo(Bullet->GetRootComponent(), EndBullet,
		GetComponentRotation(), false, false, 0.1f,
		false, EMoveComponentAction::Move, LatentInfo);

	// 총알 삭제
	Bullet->Destroy();
}

void UWeaponComponent::OnMoveFinished(bool color, FVector EndImpactPoint)
{
	UParticleSystem* particle = color? VFX_Blue :  VFX_Orange;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),particle, EndImpactPoint);
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
	if (IsValid(GrabbedObject))
	{
		
		//GrabbedObject->GetOwner();
	}
}



