// Fill out your copyright notice in the Description page of Project Settings.


#include "MPItem.h"

#include "../Player/PlayerCharacter.h"
#include "../Player/UE11PlayerController.h"
#include "../Player/UE11PlayerState.h"
#include "../Particle/ParticleCascade.h"

// Sets default values
AMPItem::AMPItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MPItemRoot"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MPItemMesh"));
	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MPItemSphere"));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MPAsset(TEXT("StaticMesh'/Game/FBX/wineWhite.wineWhite'"));
	if (MPAsset.Succeeded())
	{
		mMesh->SetStaticMesh(MPAsset.Object);
	}

	mMesh->SetRelativeScale3D(FVector(20.f, 20.f, 20.f));
	mSphere->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	mMesh->SetupAttachment(RootComponent);
	mSphere->SetupAttachment(mMesh);
	mSphere->SetCollisionProfileName(TEXT("Item"));
	mMesh->SetCollisionProfileName(TEXT("Item"));

	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AMPItem::OnPlayerOverlap);
}

// Called when the game starts or when spawned
void AMPItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMPItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMPItem::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(PlayerController))
	{
		// 플레이어를 가져온다
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());

		if (IsValid(PlayerCharacter))
		{
			// 아이템 사용에 대한 작업을 시행하는 함수 호출
			PlayerCharacter->SetPlayerMPMax();
			PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT(" MPMax!!! ")));

			FActorSpawnParameters	SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FVector	Dir = GetActorLocation() - PlayerCharacter->GetActorLocation();
			Dir.Normalize();
			FVector	ParticleLoc = PlayerCharacter->GetActorLocation() + Dir * 50.f;

			AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);
			Particle->SetActorScale3D(FVector(5.f, 5.f, 5.f));
			Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/PlayerBuffs/P_CarriedBuff_GroundPickup_Blue.P_CarriedBuff_GroundPickup_Blue'"));
			Particle->SetSound(TEXT("SoundWave'/Game/Sound/Power-Up-Sound.Power-Up-Sound'"));
		
		}
	}

	Destroy();
}


