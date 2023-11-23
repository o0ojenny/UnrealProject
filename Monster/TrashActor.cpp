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
        // ī�޶� Ʈ������ ���ϱ�
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // MuzzleOffset ī�޶� �����̽� -> ���� �����̽��� ��ȯ
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
                // �߻� ����
                FVector LaunchDirection = MuzzleRotation.Vector();
                Projectile->ThrowInDirection(LaunchDirection);
            }
        }
    }

}

