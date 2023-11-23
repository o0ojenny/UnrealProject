// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "HPItem.h"

// Sets default values
AItemActor::AItemActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ItemActorRoot"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemSphere"));
	RootComponent = mRoot;

	mMesh->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);

	mSphere->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);*/

	//mSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnPlayerOverlap);


}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}


void AItemActor::SetMesh(const FString& Path)
{

}

