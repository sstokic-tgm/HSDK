#include "RecvTable.hpp"

RecvProp *RecvTable::getProp(int i)
{
	return (RecvProp*)((*(DWORD*)this) + i * 60);
}

int RecvTable::numProp()
{
	return *(int*)((DWORD)this + 0x4);
}