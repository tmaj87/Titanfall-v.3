#pragma once
class CoreHaxFunc
{
public:
	HINSTANCE hEngine;
	HINSTANCE hClient;
	HINSTANCE hVGui;
	HINSTANCE hVGui2;
	HINSTANCE hTier0;

	tCreateInterface g_pEngineFactory;
	tCreateInterface g_ClientFactory;
	tCreateInterface g_VGUIFactory;
	tCreateInterface g_VGUI2Factory;

	EngineClient* g_pEngine;
	CEntList* g_pEntList;
	ISurface* g_pSurface;
	IPanel* g_pIPanel;
	IBaseClientDLL* g_pClient;
	dbg* debug;
	IEngineTrace* g_pTrace;

	CoreHaxFunc();
	void initFaces();
	void hook();
	//void DoMeAFavour();
	void keyManager();
	void VectorAngles(const float *forward, float *angles);
	bool visibilityCheck(Vector &vecAbsStart, Vector &vecAbsEnd, Vector &posToWrite);
};

