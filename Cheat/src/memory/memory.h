#pragma once
#include "../includes.h"

namespace memory {
	inline uint8_t* allocKeyValuesClient = nullptr;
	inline uint8_t* allocKeyValuesEngine = nullptr;

	void SetupValues();
	void* ScanPattern(char* startAddr, int bytesToScan, const char* pattern, const char* mask);
	void* ScanForPatternInModule(const char* moduleName, const char* pattern, const char* mask);
}

