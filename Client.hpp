#pragma once

#include <Windows.h>
#include "RecvTable.hpp"

class Client
{
public:

	char *getName();
	RecvTable *getRecvTable();
	Client *getNext();
	int getClsId();
};