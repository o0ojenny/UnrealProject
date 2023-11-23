// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"
#include "PlayerInfoBase.h"

void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_PlayerInfo = Cast<UPlayerInfoBase>(GetWidgetFromName(FName(TEXT("UI_PlayerInfo"))));
	m_Aim = Cast<UAimWidget>(GetWidgetFromName(FName(TEXT("UI_AimWidget"))));
	m_Minimap = Cast<UMinimapWidget>(GetWidgetFromName(FName(TEXT("UI_MinimapWidget"))));
}

void UMainHUDBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}


void UMainHUDBase::SetHP(float Ratio)
{
	if(IsValid(m_PlayerInfo))
		m_PlayerInfo->SetHP(Ratio);
}

void UMainHUDBase::SetMP(float Ratio)
{
	if (IsValid(m_PlayerInfo))
		m_PlayerInfo->SetMP(Ratio);
}
