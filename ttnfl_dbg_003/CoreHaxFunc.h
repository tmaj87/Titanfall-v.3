#pragma once
class CoreHaxFunc
{
public:
	//static void DoMeAFavour();
	static void keyManager();
	static void VectorAngles(const float *forward, float *angles);
	static void traceLine(const Vector& start /*rcx*/, const Vector& end /*rdx*/, unsigned int mask /*r8*/, const void* ignore /*r9*/, int collisionGroup, int unk, Trace* trace);
	static DWORD64* findPatternInModule(void* var1, byte* var2, const char* var3);
	static bool CoreHaxFunc::visibilityCheck(Vector &vecAbsStart, Vector &vecAbsEnd, CBaseEntity *baseEnt);
};

