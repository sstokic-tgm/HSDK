namespace InterfaceManager
{
	class Manager
	{
	public:

		/* https://github.com/pmrowla/hl2sdk-csgo/blob/master/public/tier1/interface.h */
		class InterfaceReg
		{
		private:

			using InstantiateInterfaceFn = void*(*)();

		public:

			InstantiateInterfaceFn m_CreateFn;
			const char *m_pName;

			InterfaceReg *m_pNext;
		};

		template<typename T>
		static T *getIface(const char *modName, const char *ifaceName)
		{
			T *iface = nullptr;
			InterfaceReg *ifaceRegList;
			int partMatchLen = strlen(ifaceName);

			DWORD ifaceFn = reinterpret_cast<DWORD>(GetProcAddress(GetModuleHandleA(modName), "CreateInterface"));

			if (!ifaceFn)
				return nullptr;

			unsigned int jmpStart = (unsigned int)(ifaceFn)+4;
			unsigned int jmpTarget = jmpStart + *(unsigned int*)(jmpStart + 1) + 5;

			ifaceRegList = **reinterpret_cast<InterfaceReg***>(jmpTarget + 6);

			for (InterfaceReg *cur = ifaceRegList; cur; cur = cur->m_pNext)
			{
				if (!strncmp(cur->m_pName, ifaceName, partMatchLen) && strlen(cur->m_pName) - 3 == partMatchLen)
					iface = reinterpret_cast<T*>(cur->m_CreateFn());
			}
			return iface;
		}
	};
}