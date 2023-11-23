// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "Crunch2.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API ACrunch2 : public AMonster
{
	GENERATED_BODY()

public:
	ACrunch2();

protected:
	AActor* mMuzzleParticle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	virtual void Attack();
};
