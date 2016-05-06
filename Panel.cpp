#include "VTManager.hpp"
#include "Panel.hpp"

const char *Panel::getName(unsigned int vguiPanel)
{
	typedef const char *(__thiscall* o_GetName)(void*, unsigned int);
	return VTManager::vfunc<o_GetName>(this, 36)(this, vguiPanel);
}