#pragma once
#include "../netvars/netvars.h"

class CEntity {
public:
	NETVAR(Spotted, "CBaseEntity->m_bSpotted", bool);
	NETVAR(isScoped, "CBaseEntity->m_bIsScoped", bool);
};