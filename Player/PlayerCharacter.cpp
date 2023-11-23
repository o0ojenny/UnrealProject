// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "UE11PlayerState.h"
#include "PlayerAnimInstance.h"
#include "../Skill/SkillProjectile.h"
#include "../Skill/AttackProjectile.h"
#include "../Item/WeaponActor.h"
#include "UE11PlayerController.h"
#include "../Material/UE11PhysicalMaterial.h"
#include "../Particle/ParticleCascade.h"
#include "../UE11SaveGame.h"
#include "../UE11GameModeBase.h"
#include "../UMG/MainHUDBase.h"
#include "../Monster/Monster.h"
#include "../UMG/PlayerInfoBase.h"
#include <Components\WidgetComponent.h>
#include "../Skill/TankProjectile.h"

// Sets default values 
 APlayerCharacter::APlayerCharacter()
	 : mHPRatio(1.f)
	 , mMPRatio(0.1f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerWidgetCom"));
	mWidgetComponent->SetupAttachment(GetMesh());

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	mSpringArm->SetupAttachment(GetMesh());
	mCamera->SetupAttachment(mSpringArm);

	mSpringArm->TargetArmLength = 500.f;
	mSpringArm->SetRelativeLocation(FVector(0.0, -10.0, 500.0));
	mSpringArm->SetRelativeRotation(FRotator(-15.0, 90.0, 0.0));

	mMoveDir = 0.f;

	mDeath = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mHitActor = nullptr;

	SetCanBeDamaged(true);

	mWeapon = nullptr;

	mAttackEnable = true;

	//GetMesh()->SetRenderCustomDepth(true);
	//GetMesh()->SetCustomDepthStencilValue(12);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	//UUE11SaveGame* LoadGame = Cast<UUE11SaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Save"), 0));

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	//FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Save.txt");

	//TSharedPtr<FArchive>	Reader = MakeShareable(IFileManager::Get().CreateFileReader(*FullPath));

	/*if (Reader.IsValid())
	{
		*Reader.Get() << State->mPlayerInfo.Name;
		*Reader.Get() << State->mPlayerInfo.AttackPoint;
		*Reader.Get() << State->mPlayerInfo.ArmorPoint;
		*Reader.Get() << State->mPlayerInfo.HP;
		*Reader.Get() << State->mPlayerInfo.HPMax;
		*Reader.Get() << State->mPlayerInfo.MP;
		*Reader.Get() << State->mPlayerInfo.MPMax;
		*Reader.Get() << State->mPlayerInfo.Level;
		*Reader.Get() << State->mPlayerInfo.Exp;
		*Reader.Get() << State->mPlayerInfo.Gold;
		*Reader.Get() << State->mPlayerInfo.MoveSpeed;
		*Reader.Get() << State->mPlayerInfo.AttackDistance;

		*Reader.Get() << State->mCameraZoomMin;
		*Reader.Get() << State->mCameraZoomMax;
	}*/
		

	/*State->mPlayerInfo = LoadGame->mPlayerInfo;
	State->mCameraZoomMin = LoadGame->mCameraZoomMin;
	State->mCameraZoomMax = LoadGame->mCameraZoomMax;*/
	
	State->mPlayerInfo.AttackPoint = 80;
	State->mPlayerInfo.ArmorPoint = 60;
	State->mPlayerInfo.HP = 1000;
	State->mPlayerInfo.HPMax = 1000;
	State->mPlayerInfo.MP = 50;
	State->mPlayerInfo.MPMax = 500;

	State->mPlayerInfo.Level = 1;
	State->mPlayerInfo.Gold = 10000;
	State->mPlayerInfo.Exp = 0;
	State->mPlayerInfo.MoveSpeed = 2000.f;
	State->mPlayerInfo.AttackDistance = 700.f;

	//PrintViewport(5.f, FColor::Red, FString::Printf(TEXT(" HP : %.5f"), (float)State->mPlayerInfo.HP));

}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//SavePlayer();

	LOG(TEXT("EndPlay"));

	switch (EndPlayReason)
	{
	case EEndPlayReason::Destroyed:
		LOG(TEXT("Destroyed"));
		break;
	case EEndPlayReason::LevelTransition:
		LOG(TEXT("Level Transition"));
		break;
	case EEndPlayReason::EndPlayInEditor:
		LOG(TEXT("EndPlayInEditor"));
		break;
	case EEndPlayReason::RemovedFromWorld:
		LOG(TEXT("RemovedFromWorld"));
		break;
	case EEndPlayReason::Quit:
		LOG(TEXT("Quit"));
		break;
	}
}

void APlayerCharacter::UnPossessed()
{
	Super::UnPossessed();

	LOG(TEXT("UnPossessed"));

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UI 세팅
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	AUE11GameModeBase* GameMode = Cast<AUE11GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
	//UPlayerInfoBase* PlayerWidget = Cast<UPlayerInfoBase>(mWidgetComponent->GetWidget());
	mHPRatio = (float)State->mPlayerInfo.HP / (float)State->mPlayerInfo.HPMax;
	mMPRatio = (float)State->mPlayerInfo.MP / (float)State->mPlayerInfo.MPMax;

	if (IsValid(MainHUD))
	{
		MainHUD->SetHP(mHPRatio);
		MainHUD->SetMP(mMPRatio);
	}

	AMonster* Monster = Cast<AMonster>(Controller->GetPawn());
	const FMonsterInfo& Info = Monster->GetMonsterInfo();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveFront"), this, &APlayerCharacter::MoveFront);

	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveSide"), this, &APlayerCharacter::MoveSide);

	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraZ"), this, &APlayerCharacter::RotationCameraZ);

	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("RotationCameraY"), this, &APlayerCharacter::RotationCameraY);

	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("CameraZoom"), this, &APlayerCharacter::CameraZoom);

	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("NormalAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::NormalAttack);

	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::JumpKey);

	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Skill"), EInputEvent::IE_Pressed, this, &APlayerCharacter::SkillKey);

	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("DetachWeapon"), EInputEvent::IE_Pressed, this, &APlayerCharacter::WeaponDetach);

	FInputActionBinding& toggle = PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &APlayerCharacter::InventoryOn);
	
	toggle.bConsumeInput = false;
}

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

void APlayerCharacter::MoveFront(float Scale)
{
	if (mDeath)
		return;

	mMoveDir = Scale;

	if (Scale == 0.f)
		return;

	AddMovementInput(GetActorForwardVector(), Scale);

	/*FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Scale * GetWorld()->GetDeltaSeconds();
	AddActorLocalOffset(DeltaLocation, true);*/
}

void APlayerCharacter::MoveSide(float Scale)
{

	if (mDeath)
		return;

	if (mMoveDir == 1.f)
	{
		// w키를 누른 상태에서 a, d는 없는 상태.
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);

		// w키를 누른 상태에서 오른쪽으로 이동.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(45.f);

		// w키를 누른 상태에서 왼쪽으로 이동.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-45.f);
	}

	else if (mMoveDir == -1.f)
	{
		// s키를 누른 상태에서 a, d는 없는 상태.
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(180.f);

		// s키를 누른 상태에서 오른쪽으로 이동.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(135.f);

		// s키를 누른 상태에서 왼쪽으로 이동.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-135.f);
	}

	else
	{
		// 아무 키도 누른게 없을 경우
		if (Scale == 0.f)
			mAnimInst->SetMoveDir(0.f);

		// 오른쪽으로 이동.
		else if (Scale == 1.f)
			mAnimInst->SetMoveDir(90.f);

		// 왼쪽으로 이동.
		else if (Scale == -1.f)
			mAnimInst->SetMoveDir(-90.f);
	}

	if (Scale == 0.f)
		return;

	AddMovementInput(GetActorRightVector(), Scale);
}

void APlayerCharacter::RotationCameraZ(float Scale)
{
	if (mDeath)
		return;

	if (Scale == 0.f)
		return;

	//mSpringArm->bUsePawnControlRotation = true;
	AddControllerYawInput(Scale * 100.f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::RotationCameraY(float Scale)
{
	if (Scale == 0.f)
		return;

	mSpringArm->AddRelativeRotation(FRotator(Scale * 100.f * GetWorld()->GetDeltaSeconds(), 0.f, 0.f));
}


void APlayerCharacter::CameraZoom(float Scale)
{
	if (Scale == 0.f)
		return;

	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Scale : %.5f"), Scale));

	mSpringArm->TargetArmLength += Scale * -5.f;

	// Cast : 해당 타입일 경우 해당 메모리 주소를 형변환하여 반환하고
	// 아닐 경우 nullptr을 반환한다.
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	float	CameraZoomMin = 100.f, CameraZoomMax = 1000.f;

	// State가 있을 경우 해당 값을 받아서 사용한다.
	if (IsValid(State))
	{
		CameraZoomMin = State->GetCameraZoomMin();
		CameraZoomMax = State->GetCameraZoomMax();
	}

	if (mSpringArm->TargetArmLength < CameraZoomMin)
		mSpringArm->TargetArmLength = CameraZoomMin;

	else if (mSpringArm->TargetArmLength > CameraZoomMax)
		mSpringArm->TargetArmLength = CameraZoomMax;
}

void APlayerCharacter::NormalAttack()
{
	PrintViewport(1.f, FColor::Blue, TEXT("Attack"));

	AUE11PlayerController* PlayerController = Cast<AUE11PlayerController>(GetController()); 
	
	if (mDeath)
		return;

	mAnimInst->Attack();

	/*if (IsValid(PlayerController))
	{
		PlayerController->SpawnBulletHoles();
	}*/

}

void APlayerCharacter::JumpKey()
{
	if (mDeath)
		return;

	else if (mAnimInst->GetPlayerAnimType() != EPlayerAnimType::Ground)
		return;

	Jump();
	mAnimInst->Jump();
}

void APlayerCharacter::SkillKey()
{

	UseSkill();
}


void APlayerCharacter::WeaponDetach()
{
	if (IsValid(mWeapon))
	{
		PrintViewport(1.f, FColor::Red, TEXT("detach"));
		mWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		mWeapon = nullptr;
	}
}

void APlayerCharacter::Skill()
{
	if (mDeath)
		return;

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	if (State->mPlayerInfo.MP >= State->mPlayerInfo.MPMax)
	{
		mAnimInst->UseSkill();
		State->mPlayerInfo.MP = 0;
	}

	else
		return;
}


void APlayerCharacter::InventoryOn()
{

}


void APlayerCharacter::LevelUp(int32 Level)
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	State->mPlayerInfo.Level = Level;
}

void APlayerCharacter::NormalAttackCheck()
{

}


void APlayerCharacter::UseSkill()
{

}

void APlayerCharacter::FootStep(bool Left)
{
	FVector	LineStart;

	if (Left)
		LineStart = GetMesh()->GetSocketLocation(TEXT("Finger_04_L"));

	else
		LineStart = GetMesh()->GetSocketLocation(TEXT("Finger_04_R"));

	FVector	LineEnd = LineStart + FVector::DownVector * 50.f;

	FCollisionQueryParams	param(NAME_None, false, this);

	// 충돌 결과로 물리적인 재질 여부를 가지고 올지를 결정한다.
	param.bReturnPhysicalMaterial = true;

	FHitResult	result;
	bool Hit = GetWorld()->LineTraceSingleByChannel(result, LineStart, LineEnd, ECollisionChannel::ECC_GameTraceChannel9, param);

	if (Hit)
	{
		UUE11PhysicalMaterial* Phys = Cast<UUE11PhysicalMaterial>(result.PhysMaterial);

		if (IsValid(Phys))
		{
			//PrintViewport(1.f, FColor::Blue, TEXT("Phys"));
			//Phys->Play(result.ImpactPoint, result.ImpactNormal);
			if (IsValid(Phys->GetParticle()))
			{
				FActorSpawnParameters	SpawnParam;

				SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AParticleCascade* Particle = GetWorld()->SpawnActor<AParticleCascade>(result.ImpactPoint, result.ImpactNormal.Rotation(), SpawnParam);

				Particle->SetParticle(Phys->GetParticle());
				Particle->SetSound(TEXT("SoundWave'/Game/Sound/Woosh-Sound.Woosh-Sound'"));
			}

			else if (IsValid(Phys->GetSound()))
			{
				PrintViewport(1.f, FColor::Blue, TEXT("Sound"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), Phys->GetSound(), result.ImpactPoint);
			}
		}
	}
}


void APlayerCharacter::SavePlayer()
{
	UUE11SaveGame* SaveGame = NewObject<UUE11SaveGame>();

	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());

	SaveGame->mPlayerInfo = State->mPlayerInfo;
	SaveGame->mCameraZoomMin = State->mCameraZoomMin;
	SaveGame->mCameraZoomMax = State->mCameraZoomMax;
	
	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Save"), 0);
}

float APlayerCharacter::GetPlayerHP()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	return State->mPlayerInfo.HP;
}

float APlayerCharacter::GetPlayerHPMax()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	return State->mPlayerInfo.HPMax;
}

float APlayerCharacter::GetPlayerMP()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	return State->mPlayerInfo.MP;
}

float APlayerCharacter::GetPlayerMPMax()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	return State->mPlayerInfo.MPMax;
}

void APlayerCharacter::SetPlayerHPMax()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	State->mPlayerInfo.HP = State->mPlayerInfo.HPMax;
}

void APlayerCharacter::SetPlayerMPMax()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	State->mPlayerInfo.MP = State->mPlayerInfo.MPMax;
}

void APlayerCharacter::SetPlayerAttackedHP(float Damage)
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	State->mPlayerInfo.HP -= Damage;
}

void APlayerCharacter::InitMP()
{
	AUE11PlayerState* State = Cast<AUE11PlayerState>(GetPlayerState());
	State->mPlayerInfo.MP = 0;
}

