// Fill out your copyright notice in the Description page of Project Settings.


#include "APickupRifle.h"

#include "FirstPersonCharacter.h"
#include "WeaponComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"


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

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APickupRifle::OnSphereCompOverlap);
	
}

// Called when the game starts or when spawned
void APickupRifle::BeginPlay()
{
	Super::BeginPlay();
	
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
	AFirstPersonCharacter* player = Cast<AFirstPersonCharacter*>(OtherActor);

	
	if (player != nullptr)
	{
		UWeaponComponent*  WeaponCP = NewObject<UWeaponComponent>(player);
		WeaponCP->SetupAttachment(player->GetRootComponent());

		

		
		WeaponCP->RegisterComponent();
		WeaponCP->AttachToComponent(OtherActor->GetRootComponent(),  FAttachmentTransformRules::KeepRelativeTransform);
		player->
		
		// 플레이어에게 weapon cp를 붙이기
		
	
	}


			

		// weaponcp를 플레이어의 weaponcp변수로 set
		
		

		
	}
	

		

	
}


