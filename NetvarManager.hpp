#pragma once

#include <unordered_map>
#include "BaseClientDll.hpp"

namespace NetvarManager
{
	class Manager
	{
	public:

		~Manager() { this->tables.clear(); }

		void init();
		int getOffs(const char *tableName, const char *propName);
		void dump();

	private:

		std::unordered_map<std::string, RecvTable*> tables;

		int getProp(const char *tableName, const char *propName, RecvProp **prop = 0);
		int getProp(RecvTable *primitiTabella, const char *propName, RecvProp **prop = 0);
		RecvTable *getTable(const char *tableName);
		void dumpTable(RecvTable *table, std::string tabs);
		std::string type2str(Type t);
	};
}