// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "ItemActor.h"
#include "GameFramework/Actor.h"
#include "MPItem.generated.h"

UCLASS()
class UE11_API AMPItem : public AItemActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:
	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
