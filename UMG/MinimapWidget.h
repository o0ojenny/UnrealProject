// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE11_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;
	
};
