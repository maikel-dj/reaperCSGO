#include "memory.h"

void* memory::ScanPattern(char* startAddr, int bytesToScan, const char* pattern, const char* mask) {
	int patternLen = strlen(mask);

	for (int i = 0; i < bytesToScan - patternLen; i++) {
		bool found = true;

		for (int j = 0; j < patternLen; j++) {
			if (mask[j] != '?' && pattern[j] != *(startAddr + i + j)) {
				found = false;
				break;
			}
		}

		if (found) {
			return (void*)(startAddr + i);
		}
	}

	return nullptr;
}

void* memory::ScanForPatternInModule(const char* moduleName, const char* pattern, const char* mask) {
	HMODULE hModule = GetModuleHandle(moduleName);
	if (!hModule) return nullptr;

	MODULEINFO modInfo = { 0 };
	if (!GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(modInfo))) return nullptr;

	return memory::ScanPattern((char*)modInfo.lpBaseOfDll, (int)modInfo.SizeOfImage, pattern, mask);
}

void memory::SetupValues() {
	allocKeyValuesClient = (uint8_t*)ScanForPatternInModule("client.dll", "\x55\x8B\xEC\x56\x57\x8B\xF9\x8B\xF2\x83\xFF\x0E\x0F\x87\x00\x00\x00\x00\x85\xF6", "xxxxxxxxxxxxxx????xx");
	allocKeyValuesEngine = (uint8_t*)ScanForPatternInModule("engine.dll", "\x55\x8B\xEC\x56\x57\x8B\xF9\x8B\xF2\x83\xFF\x0E\x0F\x87\x00\x00\x00\x00\x85\xF6", "xxxxxxxxxxxxxx????xx");
}