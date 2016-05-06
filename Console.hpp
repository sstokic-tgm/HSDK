#pragma once

#include "Color.hpp"
#include "VTManager.hpp"

class Console
{
public:

	template<typename ...A>
	void const ConsoleColorPrintf(const Color &c, const char *fmt, const A &...args)
	{
		typedef void(__cdecl *o_ConsoleColorPrintf)(void*, const Color&, const char*, ...);
		VTManager::vfunc<o_ConsoleColorPrintf>(this, 25)(this, c, fmt, args...);
	}
};

extern Console *p_Console;