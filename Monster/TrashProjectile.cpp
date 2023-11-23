// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashProjectile.h"
#include "../Player/UE11PlayerController.h"
#include "../Player/PlayerCharacter.h"
#include "../Particle/ParticleCascade.h"

// Sets default values
ATrashProjectile::ATrashProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TrashSphereComponent"));
	mSphere->SetCollisionProfileName(TEXT("ProjecTile"));
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &ATrashProjectile::OnPlayerOverlap);
	
	mSphere->InitSphereRadius(3000.f);
    RootComponent = mSphere;

    mMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    mMovement->SetUpdatedComponent(mSphere);
    mMovement->InitialSpeed = 1500.f;
    mMovement->MaxSpeed = 1500.f;
    mMovement->bRotationFollowsVelocity = true;
    mMovement->bShouldBounce = true;
    mMovement->Bounciness = 0.3f;

    InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ATrashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrashProjectile::ThrowInDirection(const FVector& Direction)
{
    mMovement->Velocity = Direction * mMovement->InitialSpeed;
}

void ATrashProjectile::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(PlayerController))
	{
		// 플레이어를 가져온다
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());

		if (OtherActor != this && OverlappedComp->IsSimulatingPhysics())
		{
			// HP감소
			PlayerCharacter->SetPlayerAttackedHP(50);

			PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("Dmg : %.5f"), 50));

			/*FActorSpawnParameters	SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FVector	Dir = GetActorLocation() - PlayerCharacter->GetActorLocation();
			Dir.Normalize();
			FVector	ParticleLoc = PlayerCharacter->GetActorLocation() + Dir;

			AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);
			Particle->SetActorScale3D(FVector(5.f, 5.f, 5.f));

			Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/PlayerBuffs/P_CarriedBuff_GroundPickup.P_CarriedBuff_GroundPickup'"));*/
		}
	}
}

