// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotPlayer.h"
#include "UE11PlayerState.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/ParticleNiagara.h"
#include "PlayerAnimInstance.h"
#include "../Skill/SkillProjectile.h"
#include "../Skill/AttackProjectile.h"
#include "../Particle/Decal.h"
#include "../Item/WeaponActor.h"
#include "../Skill/Ghost.h"
#include "../UE11GameModeBase.h"
#include "../UMG/MainHUDBase.h"
#include "../UE11SaveGame.h"

ARobotPlayer::ARobotPlayer()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>	RobotAsset(TEXT("SkeletalMesh'/Game/BattleRobot/Mesh/SK_BattleRobot.SK_BattleRobot'"));

	if (RobotAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(RobotAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -88.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimClass(TEXT("AnimBlueprint'/Game/Player/Animation/ABRobot.ABRobot_C'"));

	if (AnimClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimClass.Class);

	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCrunch/FX/Particles/Abilities/Uppercut/FX/P_Crunch_GutPunch_Impact.P_Crunch_GutPunch_Impact'"));


}

void ARobotPlayer::BeginPlay()
{
	Super::BeginPlay();

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	State->mPlayerInfo.Name = TEXT("Robot");

	// SkillProjectile 설정
	FSkillInfo	SkillInfo;
	SkillInfo.Damage = 500;

	ASkillProjectile* SkillProjectile = NewObject<ASkillProjectile>(this, ASkillProjectile::StaticClass());
	SkillInfo.SkillActor = Cast<ASkillActor>(SkillProjectile);

	SkillProjectile->SetStaticMesh(TEXT("StaticMesh'/Game/ParagonYin/FX/Meshes/Environment/Animals/SM_Bat.SM_Bat'"));
	SkillProjectile->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Ultimate/FX/p_Ult_EnterBubble.p_Ult_EnterBubble'"));
	SkillProjectile->SetSound(TEXT("SoundWave'/Game/Sound/Fire4.Fire4'"));
	SkillProjectile->SetCollisionProfile(TEXT("PlayerAttack"));
	SkillProjectile->mOnSkillEnd.AddDynamic(this, &ARobotPlayer::SkillEnd);

	UProjectileMovementComponent* Projectile = SkillProjectile->GetProjectile();

	Projectile->InitialSpeed = 1000.f;
	Projectile->ProjectileGravityScale = 0.f;

	//ADecal* Decal = NewObject<ADecal>(this, ADecal::StaticClass());

	//Decal->SetDecalMaterial(TEXT("Material'/Game/Materials/MTBurn.MTBurn'"));
	//Decal->SetSpawnType(EDecalSpawnType::Floor);
	//SkillProjectile->SetDecalLifeSpan(5.f);

	//SkillProjectile->SetDecalTemplate(Decal);
	
}

void ARobotPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARobotPlayer::NormalAttackCheck()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	FVector	StartLocation = GetActorLocation() + GetActorForwardVector() * 200.f;
	FVector	EndLocation = StartLocation + GetActorForwardVector() * State->GetInfo().AttackDistance;

	FCollisionQueryParams	param(NAME_None, false, this);

	TArray<FHitResult>	CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult, StartLocation, EndLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(50.f), param);

	// #define으로 ENABLE_DRAW_DEBUG 가 되어 있는지 체크한다.
#if ENABLE_DRAW_DEBUG

	// CollisionEnable 가 true이면 Red, false이면 Green을 저장한다.
	FColor	DrawColor = CollisionEnable ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), (StartLocation + EndLocation) / 2.f, State->GetInfo().AttackDistance / 2.f, 0.f,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 0.5f);

#endif

	if (CollisionEnable)
	{
		int32	Count = CollisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			FActorSpawnParameters	SpawnParam;
			//SpawnParam.Template = mHitActor;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AAttackProjectile* Attack = GetWorld()->SpawnActor<AAttackProjectile>(GetActorLocation() + GetActorForwardVector() * 100.f, GetActorRotation(), SpawnParam);
			AParticleNiagara* Particle = GetWorld()->SpawnActor<AParticleNiagara>(CollisionResult[i].ImpactPoint, CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);

			Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCrunch/FX/Particles/Abilities/Uppercut/FX/P_Crunch_GutPunch_Impact.P_Crunch_GutPunch_Impact'"));
			Particle->SetSound(TEXT("SoundWave'/Game/Sound/Laser-gun-Balttle-2.Laser-gun-Balttle-2'"));

			CollisionResult[i].GetActor()->TakeDamage((float)State->mPlayerInfo.AttackPoint, FDamageEvent(), GetController(), this);

			State->mPlayerInfo.MP += 20;

		}
	}
}
void ARobotPlayer::Skill()
{
	UseSkill();
}

void ARobotPlayer::UseSkill()
{

	FActorSpawnParameters	SpawnParam;
	//SpawnParam.Template = mSkillInfoArray[i].SkillActor;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASkillProjectile* Skill = GetWorld()->SpawnActor<ASkillProjectile>(GetActorLocation() + GetActorForwardVector() * 200.f, GetActorRotation(), SpawnParam);

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	FVector	StartLocation = GetActorLocation() + GetActorForwardVector() * 200.f;
	FVector	EndLocation = StartLocation + GetActorForwardVector() * State->GetInfo().AttackDistance;

	FCollisionQueryParams	param(NAME_None, false, this);

	TArray<FHitResult>	CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult, StartLocation, EndLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(50.f), param);

	mAnimInst->UseSkill();

	if (State->mPlayerInfo.MP >= State->mPlayerInfo.MPMax)
	{
		if (CollisionEnable)
		{
			int32	Count = CollisionResult.Num();

			for (int32 i = 0; i < Count; ++i)
			{
				AParticleNiagara* Particle = GetWorld()->SpawnActor<AParticleNiagara>(CollisionResult[i].ImpactPoint, CollisionResult[i].ImpactNormal.Rotation(), SpawnParam);

				Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCrunch/FX/Particles/Abilities/Uppercut/FX/P_Crunch_GutPunch_Impact.P_Crunch_GutPunch_Impact'"));
				Particle->SetSound(TEXT("SoundWave'/Game/Sound/Laser-gun-Balttle-2.Laser-gun-Balttle-2'"));

				CollisionResult[i].GetActor()->TakeDamage((float)State->mPlayerInfo.AttackPoint * 2, FDamageEvent(), GetController(), this);
			}
		}
	}
	State->mPlayerInfo.MP = 0;
}

void ARobotPlayer::SkillEnd(ASkillActor* SkillActor, const FHitResult& Hit)
{
	SkillActor->Destroy();
}
