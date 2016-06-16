#pragma once
#include "header.h"

class HackMechanics
{
public:
	static void cm(void* ptr, int sequence_number, float input_sample_frametime, bool active);
	static void pt(IPanel* pThis, VPANEL vguiPanel, bool bForceRepaint, bool bAllowForce);
private:
	static VPANEL matSystemTopPanel;
	static int matSystemTopPanelWidth;
	static int matSystemTopPanelHeight;

	static void initPanel(IPanel* pThis, VPANEL vguiPanel);
	static bool isCorrectPanel(IPanel* pThis, VPANEL vguiPanel);
	static bool isAboutMeAvailable();
	static void playersLoop(VPANEL vguiPanel);
};

