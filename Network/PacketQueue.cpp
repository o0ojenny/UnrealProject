// Fill out your copyright notice in the Description page of Project Settings.


#include "PacketQueue.h"

CPacketQueue::CPacketQueue()
{
}

CPacketQueue::~CPacketQueue()
{
}

void CPacketQueue::Push(int32 PacketHeader, int32 Length, uint8* Packet)
{
}

void CPacketQueue::Pop(int32 PacketHeader, int32 Length, uint8* Packet)
{
}

bool CPacketQueue::Empty()
{
	return false;
}
