#pragma once
#include "header.h"

class HackMechanics
{
public:
	HackMechanics();
	~HackMechanics();

	static void Hooked_CreateMove(void* ptr, int sequence_number, float input_sample_frametime, bool active);
	static void pt(IPanel* pThis, VPANEL vguiPanel, bool bForceRepaint, bool bAllowForce);

	void aimAtThisPlayer(CBaseEntity* player);
private:
	static VPANEL mstp;
	static void initPanel(IPanel* pThis, VPANEL vguiPanel);
	static bool isAboutMeAvailable();
	static bool isCorrectPanel(IPanel* pThis, VPANEL vguiPanel);
	static void drawByType(CBaseEntity* player, byte type, float distFromMe, byte isEnemy, byte);
	static void playersLoop(VPANEL vguiPanel);
};

