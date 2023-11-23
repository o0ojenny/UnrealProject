// Fill out your copyright notice in the Description page of Project Settings.


#include "HPItem.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/UE11PlayerState.h"
#include "../Player/UE11PlayerController.h"
#include "../Particle/ParticleCascade.h"

// Sets default values
AHPItem::AHPItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HPItemRoot"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HPItemMesh"));
	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HPItemSphere"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> HPAsset(TEXT("StaticMesh'/Game/FBX/donutSprinkles.donutSprinkles'"));
	if (HPAsset.Succeeded())
	{
		mMesh->SetStaticMesh(HPAsset.Object);
	}
	/*mMesh->SetRelativeScale3D(FVector(50.f, 50.f, 50.f));
	mMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	mSphere->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));*/

	mMesh->SetupAttachment(RootComponent);
	mSphere->SetupAttachment(mMesh);
	mSphere->SetCollisionProfileName(TEXT("Item"));
	mMesh->SetCollisionProfileName(TEXT("Item"));

	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AHPItem::OnPlayerOverlap);

}

// Called when the game starts or when spawned
void AHPItem::BeginPlay()
{
	Super::BeginPlay();
}

void AHPItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AHPItem::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(PlayerController))
	{
		// 플레이어를 가져온다
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());

		if (IsValid(PlayerCharacter))
		{
			// 아이템 사용에 대한 작업을 시행하는 함수 호출
			PlayerCharacter->SetPlayerHPMax();
			PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT(" HPMax!!! ")));

			FActorSpawnParameters	SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			FVector	Dir = GetActorLocation() - PlayerCharacter->GetActorLocation();
			Dir.Normalize();
			FVector	ParticleLoc = PlayerCharacter->GetActorLocation() + Dir * 50.f;

			AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);
			Particle->SetActorScale3D(FVector(5.f, 5.f, 5.f));
			Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/PlayerBuffs/P_CarriedBuff_GroundPickup.P_CarriedBuff_GroundPickup'"));
			Particle->SetSound(TEXT("SoundWave'/Game/Sound/Power-Up-Sound.Power-Up-Sound'"));
		
		}
	}

	Destroy();
}



