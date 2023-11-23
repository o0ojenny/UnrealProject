// Fill out your copyright notice in the Description page of Project Settings.


#include "ReceiveThread.h"
#include "NetworkSession.h"
#include "PacketQueue.h"
#include "NetworkManager.h"

CReceiveThread::CReceiveThread()
	: mSession(nullptr)
	, mLoop(true)
{
}

CReceiveThread::~CReceiveThread()
{
}

bool CReceiveThread::Init()
{
	return true;
}

uint32 CReceiveThread::Run()
{
	do
	{
	} while (mLoop);

	return 0;
}

void CReceiveThread::Stop()
{
}

void CReceiveThread::Exit()
{
	mLoop = false;
}
