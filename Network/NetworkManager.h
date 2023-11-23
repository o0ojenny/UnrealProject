// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"

struct FThreadInfo
{
	FRunnable* Action;
	FRunnableThread* Thread;

};
class UE11_API CNetworkManager
{
private:
	CNetworkManager();
	~CNetworkManager();

private:
	TMap<FString, class CNetworkSession*> mSessionMap;
	TMap<FString, FThreadInfo*> mThreadMap;

public:
	bool Init();
	bool CheckSession(const FString& Name);
	bool Connect(const FString& Name, const FString& IPAddr, int32 Port);
	bool Close(const FString& Name);
	bool Receive(const FString& Name, int32& PacketHeader, int32 Length, uint8* Packet);
	bool Send(const FString& Name, int32& PacketHeader, int32 Length, uint8* Packet);
	class CNetworkSession* FindSession(const FString& Name);

public:
	template <typename T>
	FThreadInfo* CreateThread(const FString& Name)
	{
		if (mThreadMap.Contains(Name))
			return nullptr;

		FThreadInfo* Info = new FThreadInfo;

		Info->Action = new T;
		Info->Thread = FRunnableThread::Create(Info->Action, *Name);
		mThreadMap.Emplace(Name, Info);

		return Info;
	}
private:
	static CNetworkManager* mInst;

public:
	static CNetworkManager* GetInst()
	{
		if (!mInst)
			mInst = new CNetworkManager;

		return mInst;
	}

	static void DestroyInst()
	{
		if (mInst)
		{
			delete mInst;
			mInst = nullptr;
		}
	}
};
