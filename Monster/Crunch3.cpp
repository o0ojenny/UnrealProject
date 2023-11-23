// Fill out your copyright notice in the Description page of Project Settings.


#include "Crunch3.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleCascade.h"

ACrunch3::ACrunch3()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	mMonsterTableRowName = TEXT("Crunch3");

	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* Particle = Cast<AParticleCascade>(mHitActor);
	Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonCrunch/FX/Particles/Abilities/Cross/FX/P_Crunch_Cross_DirtTrail.P_Crunch_Cross_DirtTrail'"));
	Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);

	FConvertMaterial	Mtrl;
	Mtrl.Index = 0;

	mDissolveMtrlIndexArray.Add(Mtrl);
}

void ACrunch3::BeginPlay()
{
	Super::BeginPlay();
}

void ACrunch3::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}

void ACrunch3::UnPossessed()
{
	Super::UnPossessed();
}

void ACrunch3::Attack()
{
	AAIController* MonsterController = Cast<AAIController>(GetController());

	ACharacter* Target = Cast<ACharacter>(MonsterController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(Target))
	{
		FActorSpawnParameters	SpawnParam;
		//SpawnParam.Template = mHitActor;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector	Dir = GetActorLocation() - Target->GetActorLocation();
		Dir.Normalize();

		FVector	ParticleLoc = Target->GetActorLocation() + Dir * 50.f;

		AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonYin/FX/Particles/Yin/Abilities/Primary/FX/P_Yin_Primary_Impact.P_Yin_Primary_Impact'"));
		Particle->SetSound(TEXT("SoundWave'/Game/Sound/Laser-Cannon-Sound__1_.Laser-Cannon-Sound__1_'"));

		Target->TakeDamage((float)mInfo.AttackPoint, FDamageEvent(), GetController(), this);
	}
}
