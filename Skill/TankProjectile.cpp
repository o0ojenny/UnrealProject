// Fill out your copyright notice in the Description page of Project Settings.


#include "TankProjectile.h"

// Sets default values
ATankProjectile::ATankProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	SetRootComponent(mRoot);

	mSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	mSound = nullptr;

	mSkeletalMesh->SetupAttachment(mRoot);
	mStaticMesh->SetupAttachment(mRoot);


	mRoot->bVisualizeComponent = true;

	mProjectileMovement= CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	mProjectileMovement->SetUpdatedComponent(mRoot);
	mProjectileMovement->InitialSpeed = 1000.f;
	mProjectileMovement->MaxSpeed = 1000.f;
	mProjectileMovement->bRotationFollowsVelocity = true;
	mProjectileMovement->bShouldBounce = true;
	mProjectileMovement->Bounciness = 0.3f;
}

// Called when the game starts or when spawned
void ATankProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATankProjectile::AttackInDirection(const FVector& ShootDirection)
{
	mProjectileMovement->Velocity = ShootDirection * mProjectileMovement->InitialSpeed;
}

