// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "RobotPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API ARobotPlayer : public APlayerCharacter
{
	GENERATED_BODY()

public:
	ARobotPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void NormalAttackCheck();
	virtual void Skill();

	virtual void UseSkill();


public:
	UFUNCTION()
		void SkillEnd(class ASkillActor* SkillActor, const FHitResult& Hit);
	

};
