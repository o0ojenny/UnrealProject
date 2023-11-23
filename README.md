# UnrealProject

게임 소개
- 개발 엔진 : Unreal Engine 5.0.3 (C++와 Blueprint 이용)
- 개발 기간 : 1개월
- 개발 규모 : 1명


## DemoVideo
유튜브 링크 걸기

## Controller-Character 구조


## PlayerCharacter

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

### Camera 전환 함수

마우스 입력으로 Camera Zoom In/Out, Rotation

### Animation BP


## Monster AI
Monster AI Behavior Tree

## Monster SpawnPoint

## Item

### HP Item

### MP Item

## UI

play 화면 캡쳐 후 네모 표시해서 설명
