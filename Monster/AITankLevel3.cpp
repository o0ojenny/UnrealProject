// Fill out your copyright notice in the Description page of Project Settings.


#include "AITankLevel3.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"

AAITankLevel3::AAITankLevel3()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
	GetCapsuleComponent()->SetCapsuleRadius(100.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("AITankLevel3");

	// 파티클, 사운드
}

void AAITankLevel3::BeginPlay()
{
	Super::BeginPlay();
}

void AAITankLevel3::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// BehaviorTree, BlackboardData 설정
}

void AAITankLevel3::UnPossessed()
{
	Super::UnPossessed();
}

void AAITankLevel3::Attack()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters	SpawnParam;
		//SpawnParam.Template = mHitActor;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// 타겟과 몬스터 사이에 이펙트를 재생한다.
		FVector	Dir = GetActorLocation() - Target->GetActorLocation();
		Dir.Normalize();

		FVector	ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
		Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"));

		Target->TakeDamage((float)mInfo.AttackPoint, FDamageEvent(), GetController(), this);
	}
}

