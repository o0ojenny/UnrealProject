// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS()
class UE11_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* mMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* mRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class USphereComponent* mSphere;

	class AItemSpawnPoint* mSpawnPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	void SetMesh(const FString& Path);

	void SetSpawnPoint(class AItemSpawnPoint* SpawnPoint)
	{
		mSpawnPoint = SpawnPoint;
	}
};
