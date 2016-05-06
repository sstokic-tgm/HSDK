#include "VTManager.hpp"
#include "EngineClient.hpp"

void EngineClient::getScreenSize(int &width, int &height)
{
	typedef void(__thiscall *o_GetScreenSize)(PVOID, int&, int&);
	return VTManager::vfunc<o_GetScreenSize>(this, 5)(this, width, height);
}

const D3DXMATRIX &EngineClient::worldToScreenMatrix()
{
	typedef const D3DXMATRIX &(__thiscall *o_WorldToScreenMatrix)(PVOID);
	return VTManager::vfunc<o_WorldToScreenMatrix>(this, 37)(this);
}

bool EngineClient::isConn()
{
	typedef bool(__thiscall *o_IsConnected)(PVOID);
	return VTManager::vfunc<o_IsConnected>(this, 27)(this);
}

bool EngineClient::isIG()
{
	typedef bool(__thiscall *o_IsInGame)(PVOID);
	return VTManager::vfunc<o_IsInGame>(this, 26)(this);
}

void EngineClient::execClientCMD(const char *cmd)
{
	typedef bool(__thiscall *o_ExecClientCMD)(PVOID, const char*);
	VTManager::vfunc<o_ExecClientCMD>(this, 108)(this, cmd);
}

void EngineClient::getViewAngles(vec3_t &angles)
{
	typedef void(__thiscall *o_GetViewAngles)(PVOID, vec3_t&);
	VTManager::vfunc<o_GetViewAngles>(this, 18)(this, angles);
}

void EngineClient::setViewAngles(vec3_t &angles)
{
	typedef void(__thiscall *o_SetViewAngles)(PVOID, vec3_t&);
	VTManager::vfunc<o_SetViewAngles>(this, 19)(this, angles);
}