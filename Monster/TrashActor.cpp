// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashActor.h"
#include "TrashProjectile.h"

// Sets default values
ATrashActor::ATrashActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ATrashActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrashActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("Time: %.5f"), DeltaTime));

    int32 Random = FMath::RandRange(1, 15);

    if(Random == 3)
        Throw();

}

void ATrashActor::Throw()
{
    if (ProjectileClass)
    {
        // 카메라 트랜스폼 구하기
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // MuzzleOffset 카메라 스페이스 -> 월드 스페이스로 변환
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 15.0f;
 
        if (GetWorld())
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
        
            ATrashProjectile* Projectile = GetWorld()->SpawnActor<ATrashProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // 발사 방향
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->ThrowInDirection(LaunchDirection);
            }
        }
    }

}

