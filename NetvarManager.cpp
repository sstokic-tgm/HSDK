#include "NetvarManager.hpp"

namespace NetvarManager
{
	void Manager::init()
	{
		tables.clear();

		Client *client = p_Client->getAll();
		if (!client)
			return;

		while (client)
		{
			auto recvTable = client->m_pRecvTable;

			if (recvTable)
				tables.emplace(std::string(client->m_pNetworkName), recvTable);

			client = client->m_pNext;
		}
	}

	int Manager::getOffs(const char *tableName, const char *propName)
	{
		int offs = this->getProp(tableName, propName);

		if (!offs)
			return 0;

		return offs;
	}

	int Manager::getProp(const char *tableName, const char *propName, RecvProp **prop)
	{
		RecvTable *recvTable = this->getTable(tableName);

		if (!recvTable)
			return 0;

		int offs = this->getProp(recvTable, propName, prop);

		if (!offs)
			return 0;

		return offs;
	}

	int Manager::getProp(RecvTable *recvTable, const char *propName, RecvProp **prop)
	{
		int extrOffs = 0;

		for (int i = 0; i < recvTable->numProp(); i++)
		{
			auto recvProp = recvTable->getProp(i);
			auto recvChild = recvProp->getTable();

			if (recvChild && (recvChild->numProp() > 0))
			{
				int tmp = this->getProp(recvChild, propName, prop);

				if (tmp)
					extrOffs += (recvProp->getOffset() + tmp);
			}

			if (strcmp(recvProp->getName(), propName))
				continue;

			if (prop)
				*prop = recvProp;

			return (recvProp->getOffset() + extrOffs);
		}
		return extrOffs;
	}

	RecvTable *Manager::getTable(const char *tableName)
	{
		if (tables.empty())
			return 0;

		for (auto table : tables)
		{
			if (strcmp(table.first.c_str(), tableName) == 0)
				return table.second;
		}

		return 0;
	}

	void Manager::dump()
	{
		for (Client *client = p_Client->getAll(); client != NULL; client = client->m_pNext)
		{
			auto table = client->m_pRecvTable;
			//print("%s", client->getName());
			this->dumpTable(table, "\t");
		}
	}

	void Manager::dumpTable(RecvTable *table, std::string tabs)
	{
		for (int i = 0; i < table->numProp(); i++)
		{
			auto recvProp = table->getProp(i);

			if (recvProp->getType() == Type::DPT_Array)
				continue;

			//print("%s\tName: %s Offset: 0x%X Type: %s", tabs.c_str(), recvProp->getName(), recvProp->getOffset(), this->type2str(recvProp->getType()).c_str());

			if (recvProp->getType() == Type::DPT_DataTable)
			{
				this->dumpTable(recvProp->getTable(), tabs + "\t");
			}
		}
	}

	std::string Manager::type2str(Type t)
	{
		switch (t)
		{
		case DPT_Int:
			return "Int";
		case DPT_Float:
			return "Float";
		case DPT_Vector:
			return "Vector";
		case DPT_VectorXY:
			return "VectorXY";
		case DPT_String:
			return "String";
		case DPT_Array:
			return "Array";
		case DPT_DataTable:
			return "DataTable";
		case DPT_Int64:
			return "Int64";
		case DPT_NUMSendPropTypes:
			return "NUMSendPropTypes";
		default:
			return "Unknown Type";
		}
	}
}