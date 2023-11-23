// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackProjectile.h"

AAttackProjectile::AAttackProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AttackProjectile"));

	mProjectile->SetUpdatedComponent(RootComponent);

	mProjectile->InitialSpeed = 1000.f;
	mDistance = -1.f;
}

void AAttackProjectile::BeginPlay()
{
	Super::BeginPlay();

	mProjectile->OnProjectileStop.AddDynamic(this, &AAttackProjectile::CollisionProjectile);

	mPrevLocation = GetActorLocation();
}

void AAttackProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mDistance > -1.f)
	{
		float Dist = FVector::Distance(mPrevLocation, GetActorLocation());

		mDistance -= Dist;

		if (Dist <= 0.f)
			Destroy();

		else
			mPrevLocation = GetActorLocation();
	}
}

void AAttackProjectile::CollisionProjectile(const FHitResult& Hit)
{
	CreateDecal(Hit);

	// 등록된 모든 함수를 호출해준다.
	mOnSkillEnd.Broadcast(this, Hit);
}
