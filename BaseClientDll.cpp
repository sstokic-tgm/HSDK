#include "VTManager.hpp"
#include "BaseClientDll.hpp"

Client *BaseClientDll::getAll()
{
	typedef Client*(__thiscall *o_getAll)();
	return VTManager::vfunc<o_getAll>(this, 8)();
}