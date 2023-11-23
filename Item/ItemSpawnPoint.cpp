// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnPoint.h"
#include "ItemActor.h"
#include "HPItem.h"
#include "MPItem.h"

// Sets default values
AItemSpawnPoint::AItemSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ItemRoot"));

	SetRootComponent(mRoot);

	mRoot->bVisualizeComponent = true;

	mSpawnTime = 3.f;
	mTime = 0.f;
	mSpawnCount = 1;

	mDivideCount = 5;
	mDivideMeshVisible = true;
	mSplineLength = 0.f;
	mCellDistance = 0.f;
	mPrevDivideCount = 0;
	mPrevLength = 0.f;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	DivideMesh(TEXT("StaticMesh'/Game/Blueprints/Cube.Cube'"));

	if (DivideMesh.Succeeded())
		mDivideMesh = DivideMesh.Object;

}

void AItemSpawnPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void AItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();


	if (IsValid(mSpawnItemClass))
	{
		mSpawnCount = mSpawnCount < 1 ? 1 : mSpawnCount;

		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AItemActor* SpawnItem = GetWorld()->SpawnActor<AItemActor>(mSpawnItemClass, GetActorLocation(), GetActorRotation(), SpawnParam);

		SpawnItem->SetSpawnPoint(this);
	
		mItemArray.Add(SpawnItem);
	}
	
}

// Called every frame
void AItemSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(mSpawnItemClass))
	{
		if (mItemArray.Num() < mSpawnCount)
		{
			mTime += DeltaTime;

			if (mTime >= mSpawnTime)
			{
				mTime -= mSpawnTime;

				FActorSpawnParameters	SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				AItemActor* SpawnItem = GetWorld()->SpawnActor<AItemActor>(mSpawnItemClass, GetActorLocation(), GetActorRotation(), SpawnParam);

				SpawnItem->SetSpawnPoint(this);
				mItemArray.Add(SpawnItem);

				if (mItemArray.Num() == mSpawnCount)
					mTime = 0.f;
			}
		}
	}

}

void AItemSpawnPoint::ComputeSpline()
{
	//if (mDivideMeshVisible)
	//{
	//	if (mDivideCount != mPrevDivideCount)
	//	{
	//		int32	ComCount = mMeshArray.Num();

	//		for (int32 i = 0; i < ComCount; ++i)
	//		{
	//			if (mMeshArray[i])
	//				mMeshArray[i]->DestroyComponent();
	//		}

	//		if (mMeshArray.Num() > 0)
	//			mMeshArray.RemoveAll([](UStaticMeshComponent*) { return true; });
	//	}
	//}

	//else
	//{
	//	int32	ComCount = mMeshArray.Num();

	//	for (int32 i = 0; i < ComCount; ++i)
	//	{
	//		if (mMeshArray[i])
	//			mMeshArray[i]->DestroyComponent();
	//	}

	//	if (mMeshArray.Num() > 0)
	//		mMeshArray.RemoveAll([](UStaticMeshComponent*) { return true; });
	//}

	//mSplinePoint.RemoveAll([](FVector) { return true; });
	//mSplineRotation.RemoveAll([](FRotator) { return true; });

	//mSplineLength = mPatrolSpline->GetSplineLength();
	//mCellDistance = mSplineLength / mDivideCount;

	//FTransform	ActorTr = GetActorTransform();

	//for (int32 i = 0; i <= mDivideCount; ++i)
	//{
	//	FVector	Point = mPatrolSpline->GetLocationAtDistanceAlongSpline(mCellDistance * i, ESplineCoordinateSpace::World);

	//	mSplinePoint.Add(Point);

	//	// 얻어온 월드위치를 로컬 위치로 변경도 가능하다.
	//	FVector	LocalPoint = ActorTr.InverseTransformPosition(Point);

	//	FRotator	Rot = mPatrolSpline->GetRotationAtDistanceAlongSpline(mCellDistance * i, ESplineCoordinateSpace::World);

	//	mSplineRotation.Add(Rot);

	//	if (mDivideMeshVisible)
	//	{
	//		if (mDivideCount != mPrevDivideCount)
	//		{
	//			UStaticMeshComponent* MeshCom = NewObject<UStaticMeshComponent>(this);

	//			// Mesh의 Transform을 얻어온다.
	//			FTransform	MeshTr = MeshCom->GetRelativeTransform();

	//			MeshTr.SetLocation(LocalPoint);
	//			MeshTr.SetScale3D(FVector(0.1f, 0.1f, 0.1f));

	//			MeshCom->SetRelativeTransform(MeshTr);

	//			MeshCom->SetWorldRotation(Rot);

	//			MeshCom->SetStaticMesh(mDivideMesh);

	//			MeshCom->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//			mMeshArray.Add(MeshCom);

	//			MeshCom->SetupAttachment(mPatrolSpline);
	//		}

	//		else
	//		{
	//			mMeshArray[i]->SetRelativeLocation(LocalPoint);
	//			mMeshArray[i]->SetWorldRotation(Rot);
	//		}
	//	}
	//}

	//mPrevDivideCount = mDivideCount;
	//mPrevLength = mSplineLength;
}


