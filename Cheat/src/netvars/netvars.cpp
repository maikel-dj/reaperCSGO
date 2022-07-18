#include "netvars.h"
#include "../interfaces/interfaces.h"

#include "../../ext/valve-sdk/IClientEntityList.h"

#include <ctype.h>
#include <format>
#include <assert.h>

void netvars::SetupNetvars() {
	// loop through the linked list
	assert(interfaces::client != nullptr);

	for (auto clientClass = interfaces::client->GetAllClasses(); clientClass; clientClass = clientClass->next) {
		if (clientClass->recvTable)
			Dump(clientClass->networkName, clientClass->recvTable);
	}
}

void netvars::Dump(const char* baseClass, RecvTable* table, uint32_t offset) {
	for (int i = 0; i < table->propsCount; i++) {
		const RecvProp* prop = &table->props[i];

		if (!prop || isdigit(prop->varName[0])) continue;
		if (fnv::Hash(prop->varName) == fnv::HashConst("baseclass")) continue; // don't want to store base classes, only their props

		if (prop->recvType == SendPropType::DATATABLE && prop->dataTable && prop->dataTable->tableName[0] == 'D')
			Dump(baseClass, prop->dataTable, offset + prop->offset);

		const auto netvarName = std::format("{}->{}", baseClass, prop->varName);
		netvars::list[fnv::Hash(netvarName.c_str())] = offset + prop->offset;
	}
}