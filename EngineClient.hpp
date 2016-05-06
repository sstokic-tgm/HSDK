#pragma once

#include <d3dx9math.h>
#include "Util_Vector.hpp"

class EngineClient
{
public:

	bool isConn();
	bool isIG();
	void getScreenSize(int &width, int &height);
	const D3DXMATRIX &worldToScreenMatrix();
	void execClientCMD(const char *cmd);
	void getViewAngles(vec3_t &angles);
	void setViewAngles(vec3_t &angles);
};

extern EngineClient *p_Engine;