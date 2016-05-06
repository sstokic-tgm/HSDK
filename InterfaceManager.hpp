#pragma once

namespace InterfaceManager
{
	class Manager
	{
	public:

		template<typename T>
		static T *getIface(const char *modName, const char *ifaceName)
		{
			T *iface = nullptr;
			char tmpIface[100];

			CreateInterfaceFn ifaceFn = (CreateInterfaceFn)GetProcAddress(GetModuleHandleA(modName), "CreateInterface");
			for (int i = 1; i < 100; ++i)
			{
				sprintf(tmpIface, "%s0%i", ifaceName, i);
				iface = (T*)ifaceFn(tmpIface, 0);

				if (iface != nullptr)
					break;

				sprintf(tmpIface, "%s00%i", ifaceName, i);
				iface = (T*)ifaceFn(tmpIface, 0);

				if (iface != nullptr)
					break;
			}
			return iface;
		}

	private:

		using CreateInterfaceFn = void*(*)(const char *name, int *returnC);
	};
}