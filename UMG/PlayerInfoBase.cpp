// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInfoBase.h"
#include "../Player/UE11PlayerState.h"
#include "../Player/PlayerCharacter.h"

void UPlayerInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("HP_Bar"))));
	m_MPBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("MP_Bar"))));
}

void UPlayerInfoBase::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UPlayerInfoBase::SetHP(float Ratio)
{
	if(IsValid(m_HPBar))
		m_HPBar->SetPercent(Ratio);
}

void UPlayerInfoBase::SetMP(float Ratio)
{   
	if (IsValid(m_MPBar))
		m_MPBar->SetPercent(Ratio);
}
