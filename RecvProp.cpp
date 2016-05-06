#include "RecvProp.hpp"

char *RecvProp::getName()
{
	return (char*)*(DWORD*)this;
}

Type RecvProp::getType()
{
	return (Type)*(DWORD*)((DWORD)this + 4);
}

int RecvProp::getOffset()
{
	return *(int*)((DWORD)this + 44);
}

RecvTable *RecvProp::getTable()
{
	return (RecvTable*)*(DWORD*)((DWORD)this + 40);
}