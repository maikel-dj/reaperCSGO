#pragma once
#include "../netvars/netvars.h"

class CEntity {
public:
	NETVAR(Spotted, "CBaseEntity->m_bSpotted", bool);
	NETVAR(IsScoped, "CBaseEntity->m_bIsScoped", bool);
	NETVAR(GetTeam, "CBaseEntity->m_iTeamNum", bool);

public: // renderable virtual functions (+0x4)
	bool SetupBones(CMatrix3x4* out, std::int32_t max, std::int32_t mask, float currentTime) noexcept
	{
		using Fn = bool(__thiscall*)(void*, CMatrix3x4*, int32_t, int32_t, float);
		Fn call = (*reinterpret_cast<Fn**>(this + 0x4))[13];
		return call(this + 0x4, out, max, mask, currentTime);
	}

public: // networkable virtual functions (+0x8)
	bool IsDormant() noexcept
	{
		using Fn = bool(__thiscall*)(CEntity*);
		Fn call = (*reinterpret_cast<Fn**>(this + 0x8))[9];
		return call(this + 0x8);
	}

public: // entity virtual functions
	void* GetObserverTarget()
	{
		using Fn = void*(__thiscall*)(CEntity*);
		Fn call = (*reinterpret_cast<Fn**>(this))[294];
		return call(this);
	}

	const CVector& GetAbsOrigin() noexcept
	{
		using Fn = CVector& (__thiscall*)(CEntity*);
		Fn call = (*reinterpret_cast<Fn**>(this))[10];
		return call(this);
	}

	bool IsAlive() noexcept
	{
		using Fn = bool(__thiscall*)(CEntity*);
		Fn call = (*reinterpret_cast<Fn**>(this))[156];
		return call(this);
	}
};