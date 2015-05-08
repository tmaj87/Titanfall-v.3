#include "header.h"


ExtendedPlayerClass::ExtendedPlayerClass(CBaseEntity* thisPlayer, int thisIndex)
{
	player = thisPlayer;
	health = *(int*)(DWORD64(thisPlayer) + m_iHealth);
	maxHealth = *(int*)(DWORD64(thisPlayer) + m_iMaxHealth);
	lifeState = *(int*)(DWORD64(thisPlayer) + m_lifeState);
	//index = thisPlayer->GetIndex();
	index = thisIndex;
	teamNumber = *(int*)(DWORD64(thisPlayer) + m_iTeamNum);
	enemy = isEnemy();
}

bool ExtendedPlayerClass::isEnemy()
{
	return teamNumber != *(int*)(DWORD64(myPlayer) + m_iTeamNum);
}