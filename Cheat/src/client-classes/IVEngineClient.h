#pragma once

class IVEngineClient {
public:
	bool IsInGame() noexcept
	{
		using Fn = bool(__thiscall*)(IVEngineClient*);
		Fn call = (*reinterpret_cast<Fn**>(this))[26];
		return call(this);
	}
};