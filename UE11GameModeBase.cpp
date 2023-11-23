// Copyright Epic Games, Inc. All Rights Reserved.


#include "UE11GameModeBase.h"
#include "Player/UE11PlayerController.h"
#include "Player/UE11PlayerState.h"
#include "UE11GameInstance.h"
#include "UE11SaveGame.h"
#include "Player/PlayerCharacter.h"
#include "Player/RobotPlayer.h"
#include "Player/KnightCharacter.h"

/*
�𸮾� Ÿ�� �̸�
A : ����
U : ������Ʈ Ȥ�� �Ϲ� �𸮾� Ŭ����
F : ����ü
E : ������
*/
AUE11GameModeBase::AUE11GameModeBase()
{
	// Tick �Լ� ȣ��
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AUE11PlayerController::StaticClass();
	PlayerStateClass = AUE11PlayerState::StaticClass();
		
	// UI_MainHUD �������Ʈ Ŭ���� �� UClass ������ �����´�.
	ConstructorHelpers::FClassFinder<UUserWidget> finder(TEXT("WidgetBlueprint'/Game/UMG/UI_MainHUD.UI_MainHUD_C'"));

	if (finder.Succeeded())
	{
		m_MainHUDClass = finder.Class;
	}	

	ConstructorHelpers::FClassFinder<UPlayerInfoBase> Playerfinder(TEXT("WidgetBlueprint'/Game/UMG/UI_PlayerInfo.UI_PlayerInfo_C'"));

	if (Playerfinder.Succeeded())
	{
		m_PlayerClass = Playerfinder.Class;
	}

}

// �Ʒ� �Լ����� InitGame -> PostLogin -> BeginPlay �Լ� ������
// ȣ���� �ȴ�.
void AUE11GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	mSaveGame = NewObject<UUE11SaveGame>();

	// ���� �������Ʈ UClass ������ �̿��ؼ� ��ü�� ������.
	if (IsValid(m_MainHUDClass))
	{
		// ������ ��ü�� �ּҸ� m_MainHUD �� �޾Ƶд�.
		m_MainHUD = Cast<UMainHUDBase>(CreateWidget(GetWorld(), m_MainHUDClass));
		if (IsValid(m_MainHUD))
		{
			m_MainHUD->AddToViewport();
		}
	}

	//if (IsValid(m_PlayerClass))
	//{
	//	m_PlayerInfo = Cast<UPlayerInfoBase>(CreateWidget(GetWorld(), m_PlayerClass));
	//	if (IsValid(m_PlayerInfo))
	//	{
	//		m_PlayerInfo->AddToViewport();
	//	}
	//}

}

void AUE11GameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	switch (EndPlayReason)
	{
	case EEndPlayReason::Destroyed:
		LOG(TEXT("Destroyed"));
		break;
	case EEndPlayReason::LevelTransition:
		LOG(TEXT("Level Transition"));

		SaveGame();
		/*APlayerCharacter* PlayerCharacter;
		
		PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (IsValid(PlayerCharacter))
			PlayerCharacter->SavePlayer();*/
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

void AUE11GameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UUE11GameInstance* GameInst = GetWorld()->GetGameInstance<UUE11GameInstance>();

	//DefaultPawnClass = ARobotPlayer::StaticClass();

}

void AUE11GameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	AUE11PlayerState* State = NewPlayer->GetPlayerState<AUE11PlayerState>();

	if (IsValid(State))
		State->InitPlayer();
}

void AUE11GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUE11GameModeBase::SaveGame()
{
	//UGameplayStatics::SaveGameToSlot(mSaveGame, TEXT("Save"), 0);

	FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Save.txt");

	FArchive* Writer = IFileManager::Get().CreateFileWriter(
		*FullPath);

	if (Writer)
	{
		*Writer << mSaveGame->mPlayerInfo.Name;
		*Writer << mSaveGame->mPlayerInfo.AttackPoint;
		*Writer << mSaveGame->mPlayerInfo.ArmorPoint;
		*Writer << mSaveGame->mPlayerInfo.HP;
		*Writer << mSaveGame->mPlayerInfo.HPMax;
		*Writer << mSaveGame->mPlayerInfo.MP;
		*Writer << mSaveGame->mPlayerInfo.MPMax;
		*Writer << mSaveGame->mPlayerInfo.Level;
		*Writer << mSaveGame->mPlayerInfo.Exp;
		*Writer << mSaveGame->mPlayerInfo.Gold;
		*Writer << mSaveGame->mPlayerInfo.MoveSpeed;
		*Writer << mSaveGame->mPlayerInfo.AttackDistance;

		*Writer << mSaveGame->mCameraZoomMin;
		*Writer << mSaveGame->mCameraZoomMax;

		// ����� �����ٸ� ��Ʈ���� �ݾ��ְ� �����ؾ� �Ѵ�.
		Writer->Close();

		delete Writer;
	}
}
