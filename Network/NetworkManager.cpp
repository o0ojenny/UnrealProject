// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkManager.h"
#include "NetworkSession.h"

CNetworkManager* CNetworkManager::mInst = nullptr;

CNetworkManager::CNetworkManager()
{
}

CNetworkManager::~CNetworkManager()
{
	for (auto& ThreadInfo : mThreadMap)
	{
		ThreadInfo.Value->Action->Exit();
	}

	for (auto& Session : mSessionMap)
	{
		Session.Value->Close();
	}

	for (auto& ThreadInfo : mThreadMap)
	{
		ThreadInfo.Value->Thread->Kill();
		ThreadInfo.Value->Thread->WaitForCompletion();
		
		delete ThreadInfo.Value->Thread;
		delete ThreadInfo.Value->Action;
		delete ThreadInfo.Value;
	}

	for (auto& Session : mSessionMap)
	{
		delete Session.Value;
	}

}

bool CNetworkManager::Init()
{
	return true;
}

bool CNetworkManager::CheckSession(const FString& Name)
{
	return mSessionMap.Contains(Name);
}

bool CNetworkManager::Connect(const FString& Name, const FString& IPAddr, int32 Port)
{
	CNetworkSession* Session = nullptr;

	// 세션이 없을 경우 새로 생성
	if (!CheckSession(Name))
	{
		Session = new CNetworkSession;

		// 새로 생성된 Session을 목록에 추가
		// mSessionMap.Add(,);
		// 임시 객체 생성을 피할 수 있다.
		mSessionMap.Emplace(Name, Session);
	}

	else
		Session = FindSession(Name);

	if(Session->IsConnect())
		return false;

	return Session->Connect(IPAddr, Port);
}

bool CNetworkManager::Close(const FString& Name)
{
	// Session없으면 실패
	if (!CheckSession(Name))
		return false;
	
	CNetworkSession* Session = FindSession(Name);

	if (!Session->IsConnect())
		return false;

	Session->Close();

	delete Session; 

	mSessionMap.Remove(Name);
	
	return true;
}

bool CNetworkManager::Receive(const FString& Name, int32& PacketHeader, int32 Length, uint8* Packet)
{
	if(!CheckSession(Name))
		return false;

	CNetworkSession* Session = FindSession(Name);

	if (!Session->IsConnect())
		return false;

	return Session->Receive(PacketHeader, Length, Packet);

}

bool CNetworkManager::Send(const FString& Name, int32& PacketHeader, int32 Length, uint8* Packet)
{
	if (!CheckSession(Name))
		return false;

	CNetworkSession* Session = FindSession(Name);

	if (!Session->IsConnect())
		return false;

	return Session->Send(PacketHeader, Length, Packet);
}

CNetworkSession* CNetworkManager::FindSession(const FString& Name)
{
	if(!mSessionMap.Contains(Name))
		return nullptr;

	return mSessionMap.FindRef(Name);
}
