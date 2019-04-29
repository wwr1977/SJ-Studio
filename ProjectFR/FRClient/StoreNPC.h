#pragma once
#include "NPCBase.h"
class CStoreNPC : public CNPCBase
{
	enum 
	{
		WEAPON_STORE_NPC,
		ARMOR_STORE_NPC,
		ACCESSORIES_STORE_NPC,
		MAXSTORENPC
	}m_StoreNo;

public:
	void Init();

public:
	CStoreNPC();
	~CStoreNPC();
};

