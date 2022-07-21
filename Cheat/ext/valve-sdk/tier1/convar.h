#pragma once

#include "IConVar.h"
#include "../ICvar.h"
#include "../definitions/utlvector.h"

using CommandCallbackV1Fn = void(__cdecl*)();
using ChangeCallbackFn = void(__cdecl*)(void*, const char*, float);

class ConVar {
public:
	std::byte pad[0x4];
	ConVar* next;
	bool registered;
	const char* name;
	const char* helpString;
	int flags;
	CommandCallbackV1Fn callback;
	ConVar* parent;
	const char* defaultValue;
	char* string;
	int stringLength;
	float valueFloat;
	int valueInt;
	bool hasMin;
	float minValue;
	bool hasMax;
	float maxValue;
	UtlVector<ChangeCallbackFn>	changeCallbacks;

	const char* GetName()
	{
		using Fn = const char* (__thiscall*)(ConVar*);
		Fn call = (*reinterpret_cast<Fn**>(this))[5];
		return call(this);
	}

	float GetFloat()
	{
		std::uint32_t xored = *reinterpret_cast<std::uint32_t*>(&parent->valueFloat) ^ reinterpret_cast<std::uint32_t>(this);
		return *reinterpret_cast<float*>(&xored);
	}

	int GetInt()
	{
		return static_cast<int>(parent->valueInt ^ reinterpret_cast<int>(this));
	}

	bool GetBool()
	{
		return !!GetInt();
	}

	const char* GetString() const
	{
		char const* szValue = parent->string;
		return szValue ? szValue : "";
	}

	void SetValue(const char* value)
	{
		using Fn = void (__thiscall*)(ConVar*, const char*);
		Fn call = (*reinterpret_cast<Fn**>(this))[14];
		return call(this, value);
	}

	void SetValue(float value)
	{
		using Fn = void (__thiscall*)(ConVar*, float);
		Fn call = (*reinterpret_cast<Fn**>(this))[15];
		return call(this, value);
	}

	void SetValue(int value)
	{
		using Fn = void(__thiscall*)(ConVar*, int);
		Fn call = (*reinterpret_cast<Fn**>(this))[16];
		return call(this, value);
	}
};

