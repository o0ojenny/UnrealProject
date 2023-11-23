// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// ../는 이전 폴더를 의미한다.
#include "../CharacterBase.h"

#include "PlayerCharacter.generated.h"


UCLASS()
class UE11_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent* mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* mCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = true))
	TArray<FSkillInfo>	mSkillInfoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UCameraShakeBase> ShakeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent* mWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool	mAttackEnable;
	
	float mHPRatio;
	float mMPRatio;

	float	mMoveDir;
	class UPlayerAnimInstance* mAnimInst;
	bool	mDeath;

	AActor* mHitActor;

	class AWeaponActor* mWeapon;



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector	MuzzleOffset; // 카메라 위치에서 총구 오프셋

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<class ATankProjectile> ProjectileClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	virtual void UnPossessed();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	void MoveFront(float Scale);
	void MoveSide(float Scale);
	void RotationCameraZ(float Scale);
	void RotationCameraY(float Scale);
	void CameraZoom(float Scale);
	
	UFUNCTION()
	void NormalAttack();
	void JumpKey();
	void WeaponDetach();
	void SkillKey();
	void InventoryOn();

	void LevelUp(int32 Level);

public:
	virtual void NormalAttackCheck();

	virtual void UseSkill();

	void FootStep(bool Left);

	virtual void Skill();


public:
	virtual void SavePlayer();

public:
	UFUNCTION(BlueprintCallable, Category = State)
	float GetPlayerHP();

	UFUNCTION(BlueprintCallable, Category = State)
	float GetPlayerHPMax();

	UFUNCTION(BlueprintCallable, Category = State)
	float GetPlayerMP();

	UFUNCTION(BlueprintCallable, Category = State)
	float GetPlayerMPMax();

	UFUNCTION(BlueprintCallable, Category = State)
	void SetPlayerHPMax();
	
	UFUNCTION(BlueprintCallable, Category = State)
	void SetPlayerMPMax();
	
	UFUNCTION(BlueprintCallable, Category = State)
	void SetPlayerAttackedHP(float Damage);

	UFUNCTION(BlueprintCallable, Category = State)
	void InitMP();


};
