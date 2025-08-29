// Fill out your copyright notice in the Description page of Project Settings.


#include "APickupRifle.h"
#include "FirstPersonCharacter.h"
#include "WeaponComponent.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickupRifle::APickupRifle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// defaultscene component 붙이기
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(SceneComp);

	
	// sphere의 scale 설정 sphere radius 설정, 루트에 붙이기
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(10.f);
	SphereComp->SetWorldScale3D(FVector(5.f));
	
	// skeletal mesh 스켈레탈 메쉬 애셋 설정
	SkeletalMeshComp= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	
	// sphere의 애셋 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("/Game/Asset/PortalGun/Grip_Anim.Grip_Anim"));

	// 만약 로딩을 성공했다면
	if (GunMesh.Succeeded())
	{
		// skeletalmesh의 skeletalmesh를 업데이트 하고 싶다.
		SkeletalMeshComp->SetSkeletalMesh(GunMesh.Object);
	}

	// 충돌 설정
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//SphereComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	SkeletalMeshComp->SetGenerateOverlapEvents(false);
	SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	
}

// Called when the game starts or when spawned
void APickupRifle::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickupRifle::OnSphereCompOverlap);
	
}

// Called every frame
void APickupRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



// 충돌 설정
void APickupRifle::OnSphereCompOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//만약 상대가 플레이어라면
	auto* Player = Cast<AFirstPersonCharacter>(OtherActor);
	

	if (Player)
	{
		// 생성 (Outer는 Player)
		UWeaponComponent* NewComp = NewObject<UWeaponComponent>(
			Player,
			Player->WeaponCPClass
		);

		// 인스턴스 목록에 추가 
		Player->AddInstanceComponent(NewComp);
		NewComp->OnComponentCreated();

		// SceneComponent인 경우에만 Attach
		if (USceneComponent* SC = Cast<USceneComponent>(NewComp))
		{
			SC->AttachToComponent(Player->GetRootComponent(),
				FAttachmentTransformRules::KeepRelativeTransform);
		}

		// 등록
		NewComp->RegisterComponent();

		// 플레이어 변수에 셋팅
		Player->WeaponCP = NewComp;

		// 입력 바인딩
		if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
		{
			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				NewComp->SetupInput(EIC);
				UE_LOG(LogTemp, Warning, TEXT("WeaponComponent->SetupInput bound immediately"));
			}
			else
			{
				// 아직 InputComponent가 준비 안 됐을 수 있으니 다음 틱에 재시도
				FTimerHandle Timer;
				Player->GetWorldTimerManager().SetTimer(Timer, FTimerDelegate::CreateWeakLambda(NewComp, [NewComp, PC]()
				{
					if (!IsValid(NewComp) || !IsValid(PC)) return;
					if (UEnhancedInputComponent* EIC2 = Cast<UEnhancedInputComponent>(PC->InputComponent))
					{
						NewComp->SetupInput(EIC2);
						UE_LOG(LogTemp, Warning, TEXT("WeaponComponent->SetupInput bound (deferred)"));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("EnhancedInputComponent still not ready"));
					}
				}), 0.f, false); // 다음 프레임
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerController not ready; cannot bind input"));
		}

		
		// AIM UI 생성 -> add to viewport
		APlayerController* pc = Cast<APlayerController>(GetWorld()-> GetFirstPlayerController());
		
		if (pc)
		{
			UUserWidget* AimWidget = CreateWidget<UUserWidget>(pc, WdgAimClass);
			
			if (AimWidget)
			{
				AimWidget->AddToViewport();
			}
		}
		

		// portalgun stand -> activate

		if (PortalGunStandClass)
		{
			AActor* Found = UGameplayStatics::GetActorOfClass(this, PortalGunStandClass);
			if (Found)
			{
				if (UFunction* Fn = Found->FindFunction(FName("Activate")))
				{
					UE_LOG(LogTemp, Warning, TEXT("Activate success"));
					Found->ProcessEvent(Fn, nullptr);
				}
			}
		}

		// destroy actor
		Destroy();
	}
}


