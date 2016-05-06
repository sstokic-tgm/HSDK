#pragma once

#include "RecvProp.hpp"

class RecvTable
{
public:

	RecvProp *getProp(int i);
	int numProp();
};