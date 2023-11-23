// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11PlayerController.h"
#include "../Particle/Decal.h"
#include "../CharacterBase.h"

AUE11PlayerController::AUE11PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	//bFindCameraComponentWhenViewTarget = true;
	bShowMouseCursor = true;
}

void AUE11PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// FInputModeUIOnly
	// FInputModeGameOnly
	// FInputModeGameAndUI

	FInputModeGameAndUI	Mode;
	SetInputMode(Mode);

	//mBulletHoles = GetWorld()->SpawnActor<ADecal>(FVector::ZeroVector,FRotator::ZeroRotator);

	//mBulletHoles->SetDecalMaterial(TEXT("MaterialInstanceConstant'/Game/Texture/Bullet_Hole_Mat_Inst.Bullet_Hole_Mat_Inst'"));

	// 마우스 위에 있는 액터를 인지하는 기능 온
	bEnableMouseOverEvents = true;
}

void AUE11PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult	result;
	bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel8, false, result);

}

void AUE11PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAxis<>();
}

void AUE11PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// 플레이어 컨트롤러가 Pawn에 빙의될때 호출된다.
void AUE11PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

// 플레이어 컨트롤러가 빙의에서 해제될때 호출된다.
void AUE11PlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AUE11PlayerController::SpawnBulletHoles()
{
	FActorSpawnParameters SpawnParam;

	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ADecal* Decal = GetWorld()->SpawnActor<ADecal>(mBulletHoles->GetActorLocation(),
		FRotator(0.f, mBulletHoles->GetActorRotation().Yaw, 0.f), SpawnParam);

	Decal->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
	Decal->SetDecalMaterial(TEXT("MaterialInstanceConstant'/Game/Texture/Bullet_Hole_Mat_Inst.Bullet_Hole_Mat_Inst'"));
	Decal->SetLifeSpan(5.f);
}

//void AUE11PlayerController::SpawnMousePick()
//{
//	if (mPickActor.IsValid())
//	{
//		ACharacterBase* CharacterBase = Cast<ACharacterBase>(mPickActor.Get());
//
//		if (IsValid(CharacterBase))
//			return;
//	}
//
//	else
//		return;
//
//	FActorSpawnParameters	SpawnParam;
//	SpawnParam.SpawnCollisionHandlingOverride =
//		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	ADecal* Decal =
//		GetWorld()->SpawnActor<ADecal>(
//			mMousePick->GetActorLocation(),
//			FRotator(0.f, mMousePick->GetActorRotation().Yaw, 0.f),
//			SpawnParam);
//
//	Decal->SetActorScale3D(FVector(0.5f, 0.5f, 0.5f));
//	//Decal->SetDecalMaterial(TEXT("MaterialInstanceConstant'/Game/Materials/MTMagicCircle_Inst.MTMagicCircle_Inst'"));
//	Decal->SetSpawnType(EDecalSpawnType::Floor);
//	Decal->SetLifeSpan(5.f);
//
//	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this,
//		mMousePick->GetActorLocation());
//}
