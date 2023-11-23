// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnPoint.generated.h"

UCLASS()
class UE11_API AItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnPoint();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* mRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class AItemActor>		mSpawnItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	mSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mSpawnCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<class AItemActor*>	mItemArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mDivideCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mDivideMeshVisible;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMesh* mDivideMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FVector>	mSplinePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FRotator>	mSplineRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USplineComponent* mPatrolSpline;

	TArray<FVector>		mPatrolPointLocationArray;

	float	mSplineLength;
	float	mCellDistance;

	int32	mPrevDivideCount;
	float	mPrevLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*>	mMeshArray;

	float	mTime;

public:
	float GetSplineLength()	const
	{
		return mSplineLength;
	}

	const TArray<FVector>& GetSplinePointArray()	const
	{
		return mSplinePoint;
	}

	FVector GetSplinePoint(int32 Index)	const
	{
		return mSplinePoint[Index];
	}

	void RemoveItem(class AHPItem* Item)
	{
		mItemArray.Remove(Item);
	}

public:
	virtual void OnConstruction(const FTransform& Transform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void ComputeSpline();
};
