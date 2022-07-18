#pragma once
#include "fnvhash.h"
#include "../../ext/valve-sdk/datatable.h"

#include <unordered_map>

namespace netvars {
	inline std::unordered_map<uint32_t, uint32_t> list;

	void SetupNetvars(); 
	// Recursively dump all netvars to netvars::list
	void Dump(const char* baseClass, RecvTable* table, uint32_t offset = 0);
}

#define NETVAR(func_name, netvar, type) type& func_name() \
{ \
	static auto offset = netvars::list[fnv::HashConst(netvar)]; \
	return *reinterpret_cast<type*>(uint32_t(this) + offset); \
}

