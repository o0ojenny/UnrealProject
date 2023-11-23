// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "TrashActor.generated.h"

UCLASS()
class UE11_API ATrashActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrashActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void Throw();

	// 카메라 위치에서의 총구 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// 스폰시킬 프로젝타일 클래스
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ATrashProjectile> ProjectileClass;

};
