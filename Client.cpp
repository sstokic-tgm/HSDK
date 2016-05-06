#include "Client.hpp"

char *Client::getName()
{
	return (char*)*(DWORD*)((DWORD)this + 0x8);
}

RecvTable *Client::getRecvTable()
{
	return (RecvTable*)*(DWORD*)((DWORD)this + 0xC);
}

Client *Client::getNext()
{
	return (Client*)*(DWORD*)((DWORD)this + 0x10);
}

int Client::getClsId()
{
	return *(int*)*(DWORD*)((DWORD)this + 0x14);
}