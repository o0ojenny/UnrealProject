// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\ProgressBar.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoBase.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UPlayerInfoBase : public UUserWidget
{
	GENERATED_BODY()

private:
	//TWeakObjectPtr<class APlayerCharacter> OwnerPlayer;

	UPROPERTY()
	UProgressBar*	m_HPBar;

	UPROPERTY()
	UProgressBar*	m_MPBar;

	/*float			m_InitHPRatio;
	float			m_InitMPRatio;*/

public:

	//void SetInitHP(float _Ratio)
	//{
	//	m_InitHPRatio = _Ratio;
	//}

	//void SetInitMP(float _Ratio)
	//{
	//	m_InitMPRatio = _Ratio;
	//}

public:
	void SetHP(float Ratio);
	void SetMP(float Ratio);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;
};
