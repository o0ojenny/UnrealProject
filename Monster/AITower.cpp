// Fill out your copyright notice in the Description page of Project Settings.


#include "AITower.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"

AAITower::AAITower()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("AITower");

	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT(""));
	Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);

	FConvertMaterial	Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);
}

void AAITower::BeginPlay()
{
	Super::BeginPlay();
}

void AAITower::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 다른 BehaviorTree나 BlackboardData를 사용하면 여기에서
	// 다르게 지정해준다.

	AMonsterAIController* AI = Cast<AMonsterAIController>(NewController);

	if (IsValid(AI))
	{
		AI->SetBehaviorTree(TEXT("BehaviorTree'/Game/Tower/BTTower.BTTower'"));
		AI->SetBlackboard(TEXT("BlackboardData'/Game/Tower/BBTower.BBTower'"));
	}
}

void AAITower::UnPossessed()
{
	Super::UnPossessed();
}

void AAITower::Attack()
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
