// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	mAnimType = EMonsterAnimType::Idle;
	mHitAdditive = 0.f;
	mLoopFalse = false;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
		Monster->OnDissolve();
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
		Monster->Attack();
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
		Monster->SetAttackEnd(true);
}

//void UMonsterAnimInstance::AnimNotify_Skill1()
//{
//	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
//
//	if (IsValid(Monster))
//		Monster->Skill();
//}
//
//void UMonsterAnimInstance::AnimNotify_SkillEnd()
//{
//	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());
//
//	if (IsValid(Monster))
//		Monster->ClearCurrentSkill();
//}
