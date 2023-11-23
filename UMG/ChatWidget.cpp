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

	// StartButton�� Ŭ��, ȣ��, ��ȣ�� ��Ȳ�� �߻��� �� ȣ�� ��ų ��������Ʈ ���

	// ä�� ������ ����

	// ä�ÿ� ������ ����
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
