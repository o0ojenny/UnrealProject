// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatWidget.h"
#include "../Network/NetworkManager.h"
#include "../Network/PacketQueue.h"
#include "../Network/ReceiveThread.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mSendBtn = Cast<UButton>(GetWidgetFromName(FName(TEXT("SendButton"))));
	mInputText = Cast<UEditableTextBox>(GetWidgetFromName(FName(TEXT("InputText"))));

	// StartButton에 클릭, 호버, 언호버 상황이 발생할 때 호출 시킬 델리게이트 등록

	// 채팅 서버에 접속

	// 채팅용 스레드 생성
}

void UChatWidget::NativeDestruct()
{
}

void UChatWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
}

void UChatWidget::OnSendBtnClicked()
{
}
