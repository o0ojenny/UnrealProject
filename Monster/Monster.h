// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../CharacterBase.h"
#include <Components\WidgetComponent.h>

#include "Monster.generated.h"

struct FConvertMaterial
{
	int32	Index;
	UMaterialInstanceDynamic* Mtrl;

	FConvertMaterial()	:
		Index(-1),
		Mtrl(nullptr)
	{
	}
};

UCLASS()
class UE11_API AMonster : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMonsterInfo		mInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName				mMonsterTableRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent*  mWidgetComponent;

	float m_Ratio;

	AActor* mHitActor;

	EPatrolType	mPatrolType;

	class UMonsterAnimInstance* mAnimInst;

	class AMonsterSpawnPoint* mSpawnPoint;

	bool		mAttackEnd;

	TArray<FVector>		mPatrolPointLocationArray;
	EPatrolEndDir		mPatrolDir;
	int32				mPatrolIndex;
	int32				mPatrolIndexAdd;
	float				mPatrolWaitTime;

	bool				mPatrolEnable;

	int32				mPatrolSplineCount;
	float				mPatrolSplineLength;
	float				mPatrolCellDistance;
	float				mPatrolCurrentDistance;

	TArray<UMaterialInstanceDynamic*>	mDissolveMtrlArray;

	// Dissolve를 적용하고자 하는 Material Slot의 인덱스를 넣어준다.
	TArray<FConvertMaterial>	mDissolveMtrlIndexArray;

	float				mPatrolTime;
	float				mPatrolTimeAcc;

	bool				mDissolveEnable;
	float				mDissolve;
	float				mDissolveRange;
	float				mDissolveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				mDissolveTimeMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				mDissolveMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float				mDissolveMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool				mSkillEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FName>		mSkillNameArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FMonsterSkillInfo>	mSkillDataArray;

	int32	mUseSkillIndex;

public:
	int32 GetUseSkillIndex()	const
	{
		return mUseSkillIndex;
	}

	const FMonsterSkillInfo* GetSkillData()
	{
		if (mUseSkillIndex == -1)
			return nullptr;

		return &mSkillDataArray[mUseSkillIndex];
	}

	bool GetArrive()	const
	{
		float	Dist = 10.f + GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (mPatrolIndexAdd == 1)
			return mPatrolIndex * mPatrolCellDistance - Dist <= mPatrolCurrentDistance;

		return mPatrolIndex * mPatrolCellDistance + Dist >= mPatrolCurrentDistance;
	}

	int32 GetPatrolSplineCount()	const
	{
		return mPatrolSplineCount;
	}

	float GetPatrolSplineLength()	const
	{
		return mPatrolSplineLength;
	}

	EPatrolType GetPatrolType()	const
	{
		return mPatrolType;
	}

	bool GetPatrolEnable()	const
	{
		return mPatrolPointLocationArray.Num() >= 2;
	}

	FVector GetPatrolLocation()	const;
	FVector GetPatrolPointLocation()	const;

	float GetPatrolWaitTime()	const
	{
		return mPatrolWaitTime;
	}

	bool GetAttackEnd()	const
	{
		return mAttackEnd;
	}

	const FMonsterInfo& GetMonsterInfo()	const
	{
		return mInfo;
	}

	class UMonsterAnimInstance* GetMonsterAnimInst()	const
	{
		return mAnimInst;
	}


public:
	void SetPatrolEnable(bool Enable)
	{
		mPatrolEnable = Enable;
	}

	void SetPatrolSplineCount(int32 Count)
	{
		mPatrolSplineCount = Count;
	}

	void SetPatrolCellDistance(float Length)
	{
		mPatrolCellDistance = Length;
	}

	void SetPatrolSplineLength(float Length)
	{
		mPatrolSplineLength = Length;
	}

	void SetPatrolType(EPatrolType Type)
	{
		mPatrolType = Type;
	}

	void NextPatrolPoint()
	{
		mPatrolIndex += mPatrolIndexAdd;

		switch (mPatrolType)
		{
		case EPatrolType::Point:
			if (mPatrolIndex == mPatrolPointLocationArray.Num())
			{
				switch (mPatrolDir)
				{
				case EPatrolEndDir::Progress:
					mPatrolIndex = 0;
					break;
				case EPatrolEndDir::Repeat:
					mPatrolIndexAdd = -1;
					mPatrolIndex = mPatrolPointLocationArray.Num() - 2;
					break;
				}
			}

			else if (mPatrolIndex < 0)
			{
				mPatrolIndexAdd = 1;
				mPatrolIndex = 1;
			}
			break;
		case EPatrolType::Spline:
			if (mPatrolIndex == mPatrolSplineCount + 1)
			{
				switch (mPatrolDir)
				{
				case EPatrolEndDir::Progress:
					mPatrolIndex = 1;
					mPatrolCurrentDistance -= mPatrolSplineLength;
					break;
				case EPatrolEndDir::Repeat:
					mPatrolCurrentDistance = mPatrolSplineLength - 100.f -
						GetCapsuleComponent()->GetScaledCapsuleRadius();
					mPatrolIndexAdd = -1;
					mPatrolIndex = mPatrolSplineCount - 1;
					break;
				}
			}

			else if (mPatrolIndex < 0)
			{
				mPatrolCurrentDistance = 100.f +
					GetCapsuleComponent()->GetScaledCapsuleRadius();
				mPatrolIndexAdd = 1;
				mPatrolIndex = 1;
			}
			break;
		}
	}

	void ClearPatrolWaitTime()
	{
		mPatrolWaitTime = 0.f;
	}

	void AddPatrolWaitTime(float Time)
	{
		mPatrolWaitTime += Time;
	}

	void SetPatrolDir(EPatrolEndDir Dir)
	{
		mPatrolDir = Dir;
	}

	void SetPatrolPointLocation(const TArray<FVector>& Array)
	{
		mPatrolPointLocationArray = Array;
	}

	void SetAttackEnd(bool AttackEnd)
	{
		mAttackEnd = AttackEnd;
	}

	void SetSpawnPoint(class AMonsterSpawnPoint* SpawnPoint)
	{
		mSpawnPoint = SpawnPoint;
	}

	void OnDissolve();

public:
	virtual void OnConstruction(const FTransform& Transform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

public:
	virtual void Attack();


public:
	UFUNCTION(BlueprintCallable)
	void PauseAI();

	UFUNCTION(BlueprintCallable)
	void ReStartAI();


//
//private:
//	void UseSkill(float DeltaTime);
//
//public:
//	void ClearSkill();
//	void ClearCurrentSkill();
//	virtual void Skill();
};
