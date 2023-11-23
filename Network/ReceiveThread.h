// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

/**
 * 
 */
class UE11_API CReceiveThread : public FRunnable
{
public:
	CReceiveThread();
	~CReceiveThread();

private:
	bool mLoop;
	class CNetworkSession* mSession;

public:
	void SetSession(class CNetworkSession* Session)
	{
		mSession = Session;
	}

	void SetLoop(bool Loop)
	{
		mLoop = Loop;
	}

public:
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit();
};
