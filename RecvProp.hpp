#pragma once

#include <Windows.h>

class RecvTable;

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
} Type;

class RecvProp
{
public:

	char *getName();
	Type getType();
	int getOffset();
	RecvTable *getTable();
};