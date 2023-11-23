// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "PlayerCharacter.h"
#include "../Skill/SkillProjectile.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	mMoveDir = 0.f;

	mAttack = false;
	mAttackEnable = true;
	mAttackIndex = 0;

	mAnimType = EPlayerAnimType::Ground;

	mGround = true;
	mJumpLoop = false;

	mFallRecoveryAdditive = 0.f;

	mUseSkillNumber = -1;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
	{
		UCharacterMovementComponent* Movement = PlayerCharacter->GetCharacterMovement();

		mSpeedRatio = Movement->Velocity.Size() / Movement->MaxWalkSpeed;

	mGround = Movement->IsMovingOnGround(); // �� ���� �ִ��� �Ǵ�

	//if (!mGround && mAnimType != EPlayerAnimType::Jump)
	//	{
	//		//mAnimType = EPlayerAnimType::Fall;
	//	}

	}
}

void UPlayerAnimInstance::Attack()
{
	if (!mAttackEnable)
		return;

	// ������ �ϰ� �ִ� �߿��� �ٽ� �������� �ϱ� ���ؼ�
	// ���� �Ұ��� ���·� ������ش�.
	mAttackEnable = false;

	// Montage_IsPlaying : ���ڷ� �� ��Ÿ�ְ� ����ǰ� �ִ���
	// �Ǵ����ش�.
	if (!Montage_IsPlaying(mAttackMontageArray[mAttackIndex]))
	{
		Montage_SetPosition(mAttackMontageArray[mAttackIndex], 0.f);
		Montage_Play(mAttackMontageArray[mAttackIndex]);

		mAttackIndex = (mAttackIndex + 1) % mAttackMontageArray.Num();
		mAttack = true;
	}

	else
		mAttackEnable = true;
}

void UPlayerAnimInstance::Jump()
{
	mAnimType = EPlayerAnimType::Jump;
}

void UPlayerAnimInstance::UseSkill()
{
	if (!Montage_IsPlaying(mSkillMontageArray[mAttackIndex]))
	{
		Montage_SetPosition(mSkillMontageArray[mAttackIndex], 0.f);
		Montage_Play(mSkillMontageArray[mAttackIndex]);
	}

	else
		mAttackEnable = true;
}

void UPlayerAnimInstance::AnimNotify_Attack()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->NormalAttackCheck();
}

void UPlayerAnimInstance::AnimNotify_AttackEnable()
{
	PrintViewport(1.f, FColor::Red, TEXT("AttackEnable"));
	mAttackEnable = true;
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	PrintViewport(1.f, FColor::Red, TEXT("AttackEnd"));
	mAttackIndex = 0;
	mAttackEnable = true;
	mAttack = false;
}

void UPlayerAnimInstance::AnimNotify_JumpEnd()
{
	mAnimType = EPlayerAnimType::Fall;
}

void UPlayerAnimInstance::AnimNotify_FallEnd()
{
	mAnimType = EPlayerAnimType::Ground;

	if (IsValid(mFallRecoveryMontage))
	{
		mFallRecoveryAdditive = 1.f;

		Montage_SetPosition(mFallRecoveryMontage, 0.f);
		Montage_Play(mFallRecoveryMontage);
	}
}

void UPlayerAnimInstance::AnimNotify_FallRecoveryEnd()
{
	mFallRecoveryAdditive = 0.f;
}

void UPlayerAnimInstance::AnimNotify_UseSkill()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter) && (PlayerCharacter->GetPlayerMP()>= PlayerCharacter->GetPlayerMPMax()))
		PlayerCharacter->UseSkill();
}

void UPlayerAnimInstance::AnimNotify_FootLeft()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->FootStep(true);
}

void UPlayerAnimInstance::AnimNotify_FootRight()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PlayerCharacter))
		PlayerCharacter->FootStep(false);
}
