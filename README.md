# UnrealProject

## 게임 소개
- 개발 엔진 : Unreal Engine 5.0.3 (C++와 Blueprint 이용)
- 개발 기간 : 1개월 (2023.03 ~ 2023.04)
- 개발 규모 : 1명


## DemoVideo
[![Video Label](https://img.youtube.com/vi/LQmt_Zww4Oc/0.jpg)](https://youtu.be/LQmt_Zww4Oc)


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
- NormalAttackCheck() : Player가 현재 공격이 가능한 상태인지 체크하는 함수
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

- UseSkill() : MP가 100% 일 때 Skill 사용 가능
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

![스크린샷 2023-11-23 183822](https://github.com/o0ojenny/UnrealProject/assets/93306648/9061fb04-1b03-488f-89a6-f17e3e04f7a9)
![스크린샷 2023-11-23 183920](https://github.com/o0ojenny/UnrealProject/assets/93306648/b514d7cf-17b0-40ab-8240-f7acff38a736)

## Monster
- Data Table을 이용하여 Monster State 관리

### Monster AI Behavior Tree

![스크린샷 2023-11-23 183124](https://github.com/o0ojenny/UnrealProject/assets/93306648/230472c2-82c7-4d90-b97d-a283ac2a1d4a)

### Monster Animation BP

![스크린샷 2023-11-23 184748](https://github.com/o0ojenny/UnrealProject/assets/93306648/6cdd2aef-bcfc-4d75-8c35-60cbd2744643)
![스크린샷 2023-11-23 184814](https://github.com/o0ojenny/UnrealProject/assets/93306648/87014afd-688d-4d30-9fea-2ba660d01505)

### Monster SpawnPoint

## Item

### HP Item
- Player와 충돌하면 Player의 HP를 최대치로 올려준다.
  
```
void AHPItem::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(PlayerController))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());

		if (IsValid(PlayerCharacter))
		{
			// 아이템 사용에 대한 작업을 시행하는 함수 호출
			PlayerCharacter->SetPlayerHPMax();
			PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT(" HPMax!!! ")));

			FActorSpawnParameters	SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			FVector	Dir = GetActorLocation() - PlayerCharacter->GetActorLocation();
			Dir.Normalize();
			FVector	ParticleLoc = PlayerCharacter->GetActorLocation() + Dir * 50.f;

			AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);
			Particle->SetActorScale3D(FVector(5.f, 5.f, 5.f));
			Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/PlayerBuffs/P_CarriedBuff_GroundPickup.P_CarriedBuff_GroundPickup'"));
			Particle->SetSound(TEXT("SoundWave'/Game/Sound/Power-Up-Sound.Power-Up-Sound'"));
		
		}
	}

	Destroy();
}
```

### MP Item
- Player와 충돌하면 Player의 MP를 최대치로 올려준다.
- MP가 최대치가 되면 스킬을 사용할 수 있다.
  
```
void AMPItem::OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (IsValid(PlayerController))
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetCharacter());

		if (IsValid(PlayerCharacter))
		{
			// 아이템 사용에 대한 작업을 시행하는 함수 호출
			PlayerCharacter->SetPlayerMPMax();
			PrintViewport(5.f, FColor::Blue, FString::Printf(TEXT(" MPMax!!! ")));

			FActorSpawnParameters	SpawnParam;
			SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FVector	Dir = GetActorLocation() - PlayerCharacter->GetActorLocation();
			Dir.Normalize();
			FVector	ParticleLoc = PlayerCharacter->GetActorLocation() + Dir * 50.f;

			AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(ParticleLoc, Dir.Rotation(), SpawnParam);
			Particle->SetActorScale3D(FVector(5.f, 5.f, 5.f));
			Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonMinions/FX/Particles/PlayerBuffs/P_CarriedBuff_GroundPickup_Blue.P_CarriedBuff_GroundPickup_Blue'"));
			Particle->SetSound(TEXT("SoundWave'/Game/Sound/Power-Up-Sound.Power-Up-Sound'"));
		
		}
	}

	Destroy();
}
```

## UI

![Slide1](https://github.com/o0ojenny/UnrealProject/assets/93306648/58f27809-cc50-44a0-8dd1-46657412f75e)

- MiniMap : Camera를 상단에 설치하여 Material로 받아와 게임 UI에 적용

- Aim : 공격 시 발사체의 예상 위치를 보여줌

- HP / MP Bar : PlayerState에서 HP, MP 정보와 연결하여 UI로 보여줌
