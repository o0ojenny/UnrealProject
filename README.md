# UnrealProject

게임 소개
- 개발 엔진 : Unreal Engine 5.0.3 (C++와 Blueprint 이용)
- 개발 기간 : 1개월
- 개발 규모 : 1명


## DemoVideo
유튜브 링크 걸기

## Controller-Character 구조


## PlayerCharacter
- 게임 안에서 이동하며 공격하는 RobotPlayer의 부모 클래스
- Player State 기본값 설정
- Animation Bind
- Camera Zoom In/Out, 회전

### TakeDamage 함수
Player가 Monster에게 공격 Collision과 충돌했을 때 호출되는 함수

```
float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	PrintViewport(2.f, FColor::Red, FString::Printf(TEXT("Dmg : %.5f"), DamageAmount));

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	if (IsValid(State))
	{
		float Dmg = DamageAmount - State->mPlayerInfo.ArmorPoint;

		Dmg = Dmg < 1.f ? 1.f : Dmg;

		State->mPlayerInfo.HP -= (int32)Dmg * 10;
	}

	return Damage;
}
```

## Robot Player
- NormalAttackCheck : Player가 현재 공격이 가능한 상태인지 체크하는 함수
```
void ARobotPlayer::NormalAttackCheck()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	FVector	StartLocation = GetActorLocation() + GetActorForwardVector() * 200.f;
	FVector	EndLocation = StartLocation + GetActorForwardVector() * State->GetInfo().AttackDistance;

	FCollisionQueryParams	param(NAME_None, false, this);

	TArray<FHitResult>	CollisionResult;
	bool CollisionEnable = GetWorld()->SweepMultiByChannel(CollisionResult, StartLocation, EndLocation, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3, FCollisionShape::MakeSphere(50.f), param);

	// #define으로 ENABLE_DRAW_DEBUG 가 되어 있는지 체크
#if ENABLE_DRAW_DEBUG

	// CollisionEnable 가 true이면 Red, false이면 Green을 저장
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
```

- MP가 100% 일 때 Skill 사용 가능
```
void ARobotPlayer::UseSkill()
{

	FActorSpawnParameters	SpawnParam;
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
```
### Player Animation BP


## Monster AI
Monster AI Behavior Tree

## Monster SpawnPoint

## Item

### HP Item

### MP Item

## UI

play 화면 캡쳐 후 네모 표시해서 설명
