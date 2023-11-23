// Fill out your copyright notice in the Description page of Project Settings.


#include "UE11GameInstance.h"

#include "Network/NetworkManager.h"

UUE11GameInstance::UUE11GameInstance()
	: m_MonsterTable(nullptr)
	, m_ItemTable(nullptr)
{
	static ConstructorHelpers::FObjectFinder<UDataTable>	MonsterTable(TEXT("DataTable'/Game/Monster/MonsterTable.MonsterTable'"));
	if (MonsterTable.Succeeded())
		m_MonsterTable = MonsterTable.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable>	SkillfInfoTable(TEXT("DataTable'/Game/Skill/DTMonsterSkill.DTMonsterSkill'"));
	if (SkillfInfoTable.Succeeded())
	{
		m_MonsterSkillInfoTable = SkillfInfoTable.Object;
	}
}

UUE11GameInstance::~UUE11GameInstance()
{
}

void UUE11GameInstance::Init()
{
	Super::Init();
	CNetworkManager::GetInst()->Init();
}

const FMonsterTableInfo* UUE11GameInstance::FindMonsterTable(const FName& Name)
{
	// 2��° ������ ContextString�� �ش� Ű�� �̿��Ͽ� Ž����
	// ������ ��� ����� �޽����� FString Ÿ������ �����Ѵ�.
	return m_MonsterTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}

const FSkillData* UUE11GameInstance::FindMonsterSkillTable(const FName& Name)
{
	return m_MonsterSkillInfoTable->FindRow<FSkillData>(Name, TEXT(""));
}
