#pragma once

#include <Windows.h>
#include "RecvTable.hpp"

class IClientNetworkable;

class Client
{
private:

	using CreateClientClassFn = IClientNetworkable*(*)(int entnum, int serialNum);
	using CreateEventFn = IClientNetworkable*(*)();

public:

	CreateClientClassFn m_pCreateFn;
	CreateEventFn m_pCreateEventFn;
	char *m_pNetworkName;
	RecvTable *m_pRecvTable;
	Client *m_pNext;
	int m_ClassID;
};