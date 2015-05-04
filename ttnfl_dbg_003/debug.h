#pragma once
#include <fstream>

using namespace std;

class dbg
{
private:
	ofstream handle;
	char *name;
public:
	dbg()
	{
		name = "__debug.txt";
	}
	void cleanFile()
	{
		handle.open(name, fstream::trunc);
		handle.close();
	}
	void toFile(const char* str)
	{
		handle.open(name, fstream::app);
		handle << str << endl;
		handle.close();
	}
};


/*
typedef VPANEL (__thiscall* testFunc)(void* pThis, VPANEL vguiPanel);
testFunc func2check;
typedef const char* (__thiscall* testFunc2)(void* pThis, VPANEL vguiPanel);
testFunc2 func2check2;

static char buff2[512];
dbg* d = new dbg();

//sprintf_s(buff2, "IPanel func count: %d", VMThook->iGetFuncCount());
//d->toFile(buff2);

int funcIdxs[] = { 22, 23, 24, 25, 26, 27, 28 };
for (int i = 0; i < sizeof(funcIdxs) / sizeof(*funcIdxs); i++)
{
func2check = (testFunc)VMThook->dwGetMethodAddress(funcIdxs[i]);
sprintf_s(buff2, "%d (%d) --> %d", funcIdxs[i], vguiPanel, func2check(pThis, vguiPanel));
d->toFile(buff2);
}

int funcIdxs2[] = { 42, 43, 44 }; // 30,..38,39,40
for (int i = 0; i < sizeof(funcIdxs2) / sizeof(*funcIdxs2); i++)
{
func2check2 = (testFunc2)VMThook->dwGetMethodAddress(funcIdxs2[i]);
sprintf_s(buff2, "%d --> %s", funcIdxs2[i], func2check2(pThis, vguiPanel));
d->toFile(buff2);
}
d->toFile("");

return;
*/


//const wchar_t offStr[] = L"off";
//core->g_pSurface->DrawPrintText(offStr, wcslen(offStr));
//core->g_pSurface->DrawOutlineRectangle(0, 0, 100, 100);

/*
headBone = Vector(boneList[10][0][3], boneList[10][1][3], boneList[10][2][3]);
for (int i = 11; i < 15; i++)
{
if (boneList[i][2][3] > headBone.z)
{
headBone = Vector(boneList[i][0][3], boneList[i][1][3], boneList[i][2][3]);
}
}
*/

/*
char buff[512] = { 0 };
//sprintf_s(buff, "hEngine: %p\nhClient: %p\nhVGui: %p\nhVGui2: %p\n", hEngine, hClient, hVGui, hVGui2);
//DebugToFile(buff);
//sprintf_s(buff, "g_pEngineFactory: %p\ng_ClientFactory: %p\ng_VGUIFactory: %p\ng_VGUI2Factory: %p\n", g_pEngineFactory, g_ClientFactory, g_VGUIFactory, g_VGUI2Factory);
//DebugToFile(buff);
sprintf_s(buff, "g_pEngine: %p\ng_pEntList: %p\ng_pSurface: %p\ng_pIPanel: %p\ng_pClient: %p\n", g_pEngine, g_pEntList, g_pSurface, g_pIPanel, g_pClient, engineTrace);
debug->toFile(buff);
*/

/*

swprintf_s(buff, L"D_eb'ug");
core->g_pSurface->DrawSetTextFont(0);
core->g_pSurface->DrawSetTextColor(255, 255, 255, 200);
core->g_pSurface->DrawSetTextPos(200, 160);
core->g_pSurface->DrawPrintText(buff, wcslen(buff));
//
*/

/*
int funcIdxs[] = { 24 };
for (int i = 0; i < sizeof(funcIdxs) / sizeof(*funcIdxs); i++)
{
sprintf_s(__DEBUG_BUFF, "func %d --> %p", funcIdxs[i], VMThook2->dwGetMethodAddress(funcIdxs[i]));
core->debug->toFile(__DEBUG_BUFF);
}
*/

/*
if (!once)
{
DWORD_PTR dwFunc1 = (DWORD_PTR)oCreateMove + 0x27;
DWORD_PTR dwRenderHolder = (DWORD_PTR)((*(DWORD*)(dwFunc1 + 0x3)) + (dwFunc1 + 0x7)); // cut 4 bytes

do {
Sleep(50);
pInput = *(CInput**)dwRenderHolder;
} while (!pInput);

sprintf_s(__DEBUG_BUFF, "dwFunc1:%llX dwRenderHolder:%llX pInput:%llX", dwFunc1, dwRenderHolder, pInput);
core->debug->toFile(__DEBUG_BUFF);

cvmth64* tmpHook = new cvmth64();
if (tmpHook->bInitialize((PDWORD64*)pInput))
{
for (int i = 0; i < tmpHook->iGetFuncCount(); i++)
{
sprintf_s(__DEBUG_BUFF, "pInput method %d: %p", i, tmpHook->dwGetMethodAddress(i));
//core->debug->toFile(__DEBUG_BUFF);
}
}

once = true;
}
*/

//sprintf_s(__DEBUG_BUFF, "sequence_number: %d, input_sample_frametime: %d, active: %d", sequence_number, input_sample_frametime, active);
//core->debug->toFile(__DEBUG_BUFF);

/*

*/


/*
CVerifiedUserCmd* pVerified = pInput->GetVerifiedUserCmd(sequence_number);

if (pVerified)
{
memcpy(&pVerified->m_cmd, pCmd, sizeof(CUserCmd));

pVerified->m_crc = pCmd->GetChecksum();
}
*/


/*
struct Ray_t
{
__declspec(align(16)) Vector m_Start;
__declspec(align(16)) Vector m_Delta;
__declspec(align(16)) Vector m_StartOffset;
__declspec(align(16)) Vector m_Extents;

//const   matrix3x4_t* m_pWorldAxisTransform;

bool    m_IsRay;
bool    m_IsSwept;

//Ray_t() : m_pWorldAxisTransform(NULL) { }

void Init(Vector& vecStart, Vector& vecEnd)
{
m_Delta = vecEnd - vecStart;

m_IsSwept = (m_Delta.LengthSqr() != 0);

m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;

//m_pWorldAxisTransform = NULL;

m_IsRay = true;

m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

m_Start = vecStart;
}
};





struct cplane_t
{
Vector  normal;
float   dist;
BYTE    type;
BYTE    signbits;
BYTE    pad[2];
};

class CBaseTrace
{
public:
Vector                  startpos;
Vector                  endpos;
cplane_t                plane;
float                   fraction;
int                             contents;
unsigned short  dispFlags;
bool                    allsolid;
bool                    startsolid;
};

struct csurface_t
{
const char*             name;
short                   surfaceProps;
unsigned short  flags;
};

class CGameTrace : public CBaseTrace
{
public:
bool                    DidHitWorld() const;
bool                    DidHitNonWorldEntity() const;
int                             GetEntityIndex() const;
bool                    DidHit() const;
public:
float                   fractionleftsolid;
csurface_t              surface;
int                             hitgroup;
short                   physicsbone;
unsigned short  worldSurfaceIndex;
CBaseEntity*               m_pEnt;
int                             hitbox;
};

inline bool CGameTrace::DidHit() const
{
return fraction < 1.0f || allsolid || startsolid;
}

typedef CGameTrace trace_t;

enum TraceType_t
{
TRACE_EVERYTHING = 0,
TRACE_WORLD_ONLY,
TRACE_ENTITIES_ONLY,
TRACE_EVERYTHING_FILTER_PROPS,
};

class ITraceFilter
{
public:
virtual bool            ShouldHitEntity(PVOID pEntity, int contentsMask) = 0;
virtual TraceType_t     GetTraceType() const = 0;
};

class EngineTraceClient004
{
public:
void TraceRay(const Ray_t &ray, unsigned int fMask, void *vNull, trace_t *pTrace)
{
typedef void(__thiscall* OriginalFn)(PVOID, const Ray_t&, unsigned int, void*, trace_t*);
getvfunc<OriginalFn>(this, 4)(this, ray, fMask, NULL, pTrace); // 5?
}
};

*/

/* struct Ray_t
{
__declspec(align(16)) Vector        m_Start;
__declspec(align(16)) Vector        m_Delta;
__declspec(align(16)) Vector        m_StartOffset;
__declspec(align(16)) Vector        m_Extents;

const matrix3x4*    m_WorldAxisTransform;

bool                m_IsRay;
bool                m_IsSwept;

Ray_t(void)
: m_WorldAxisTransform(0)
{

}

void Init(const Vector& start, const Vector& end)
{
m_Delta = end - start;

m_IsSwept = (m_Delta.LengthSqr() != 0);

m_Extents.Init();

m_WorldAxisTransform = 0;

m_IsRay = true;

m_StartOffset.Init();

m_Start = start;
}
};

enum TraceType_t
{
TRACE_EVERYTHING = 0,
TRACE_WORLD_ONLY,
TRACE_ENTITIES_ONLY,
TRACE_EVERYTHING_FILTER_PROPS,
};

struct cplane_t
{
Vector  normal;
float   dist;
BYTE    type;
BYTE    signbits;
BYTE    pad[2];
};

struct csurface_t
{
const char*             name;
short                   surfaceProps;
unsigned short          flags;
};

class CBaseTrace
{
public:
Vector                  startpos;
Vector                  endpos;
cplane_t                plane;
float                   fraction;
int                     contents;
unsigned short          dispFlags;
bool                    allsolid;
bool                    startsolid;
};

class CGameTrace : public CBaseTrace
{
public:
bool                    DidHitWorld() const;
bool                    DidHitNonWorldEntity() const;
int                     GetEntityIndex() const;
bool                    DidHit() const;

public:
float                   fractionleftsolid;
csurface_t              surface;
int                     hitgroup;
short                   physicsbone;
unsigned short          worldSurfaceIndex;
CBaseEntity*            m_pEnt;
int                     hitbox;

CGameTrace() { }

private:
CGameTrace(const CGameTrace& vOther);
};

inline bool CGameTrace::DidHit(VOID) const
{
return fraction < 1.0f || allsolid || startsolid;
}

typedef CGameTrace trace_t;

class ITraceFilter
{
public:
virtual bool ShouldHitEntity(CBaseEntity *pEntity, int contentsMask) = 0;
virtual TraceType_t     GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
CTraceFilter(CBaseEntity* pPassEntity)
{
m_pClientEntityToSkip = pPassEntity;
}

bool ShouldHitEntity(CBaseEntity* pHandleEntity, int contentsMask)
{
return !(pHandleEntity == m_pClientEntityToSkip);
}

virtual TraceType_t GetTraceType() const
{
return TRACE_EVERYTHING;
}

private:
CBaseEntity* m_pClientEntityToSkip;
};

class EngineTraceClient004
{
public:

void TraceRay(const Ray_t &ray, unsigned int fMask, CTraceFilter *pTraceFilter, trace_t *pTrace)
{
typedef void(__thiscall* OriginalFn)(PVOID, const Ray_t&, unsigned int, CTraceFilter*, trace_t*);
//VTable::CallVirtualFunction< vTraceRay >(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
getvfunc<OriginalFn>(this, 5)(this, ray, fMask, NULL, pTrace); // 5?

//typedef void(__thiscall* OriginalFn)(PVOID, const Ray_t&, unsigned int, void*, trace_t*);
//getvfunc<OriginalFn>(this, 5)(this, ray, fMask, NULL, pTrace); // 5?
}

}; */


/* bool bCompare(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
for (; *szMask; ++szMask, ++pData, ++bMask)
{
if (*szMask == 'x' && *pData != *bMask)
{
return 0;
}
}
return (*szMask) == NULL;
}*/

/* DWORD64 FindPattern(DWORD* dwAddress, DWORD dwLen, BYTE *bMask, char *szMask)
{
for (DWORD i = 0; i < dwLen; i++) {
if (bCompare((BYTE*)(dwAddress + i), bMask, szMask))
{
return (DWORD64)(dwAddress + i);
}
}
return 0;
} */