#pragma once
class HackMechanics
{
public:
	/*
	CBaseEntity* player;
	Vector playerPos, screenPos;
	player_info_s pInfo;
	float distFromMe;
	byte type, isEnemy, a;
	VPANEL mstp;
	*/


	HackMechanics();
	~HackMechanics();

	static void Hooked_CreateMove(void* ptr, int sequence_number, float input_sample_frametime, bool active);
	static void pt(IPanel* pThis, VPANEL vguiPanel, bool bForceRepaint, bool bAllowForce);

	void aimAtThisPlayer(CBaseEntity* player);
private:
	static VPANEL mstp;
	static IPanel* g_pThis;
	static VPANEL g_vguiPanel;
	static void initPanel();
	static bool isAboutMeAvailable();
	static bool isCorrectPanel();
	static void drawByType(CBaseEntity* player, byte type, float distFromMe, byte isEnemy, byte);
};

