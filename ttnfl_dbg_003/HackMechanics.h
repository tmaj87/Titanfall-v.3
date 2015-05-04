#pragma once
class HackMechanics
{
public:
	HackMechanics();
	~HackMechanics();

	static void Hooked_CreateMove(void* ptr, int sequence_number, float input_sample_frametime, bool active);
	static void pt(IPanel* pThis, VPANEL vguiPanel, bool bForceRepaint, bool bAllowForce);

	void aimAtThisPlayer(CBaseEntity* player);
};

