#pragma once
#include "vector.h"

#define IN_ATTACK (1 << 0)

inline void**& getvtable(void* inst, size_t offset = 0)
{
	return *reinterpret_cast<void***>((size_t)inst + offset);
}
inline const void** getvtable(const void* inst, size_t offset = 0)
{
	return *reinterpret_cast<const void***>((size_t)inst + offset);
}
template< typename Fn >
inline Fn getvfunc(const void* inst, size_t index, size_t offset = 0)
{
	return reinterpret_cast<Fn>(getvtable(inst, offset)[index]);
}

typedef unsigned int VPANEL;
typedef float matrix3x4[3][4];

typedef struct player_info_s {
	char    _0x0000[0x0008];
	char    szName[32];
	char    _0x0028[0x0228];
} player_info_t;

typedef void* (__cdecl* CreateInterface)(const char*, int*);
typedef double(__cdecl* Plat_FloatTime)();
typedef void* (__thiscall* PaintTraverse)(void*, VPANEL, bool, bool);
typedef void* (__thiscall* CreateMove)(void*, int, float, bool);

const DWORD64 m_iHealth = 0x1B8; //ok
const DWORD64 m_iTeamNum = 0x1D0; // ok
const DWORD64 m_localAngle = 0x1FC;
const DWORD64 m_localOrigin = 0x208;
const DWORD64 m_local = 0x26A0; // fixed
const DWORD64 m_iMaxHealth = 0x324; // ok
const DWORD64 m_lifeState = 0x488; // ok
//
const DWORD64 m_inventory = 0x23E0; // fixed
const DWORD64 m_hGroundEntity = 0x1B4; // ok
//const DWORD64 m_duckState = 0x2D14;
//const DWORD64 m_title = 0x2464;
const DWORD64 m_vecPunchBase_Angle = 0x3C;
const DWORD64 m_vecPunchBase_AngleVel = 0x48;
const DWORD64 m_vecPunchWeapon_Angle = 0x54;
const DWORD64 m_vecPunchWeapon_AngleVel = 0x4;
//
const DWORD64 m_vecViewOffset_x = 0x1E4;
const DWORD64 m_vecViewOffset_y = 0x1E8;
const DWORD64 m_vecViewOffset_z = 0x1EC;

class ClientClass
{
public:
	const char* GetName()
	{
		typedef const char* (__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 1)(this);
	}
};

class IBaseClientDLL
{
public:
};

class CViewRender {
public:
	matrix3x4* GetWorldToScreenMatrix(bool bUnknown = false)
	{
		typedef matrix3x4* (__thiscall* OriginalFn)(PVOID, bool);
		return getvfunc<OriginalFn>(this, 13)(this, bUnknown);
	}
};

class IClientRenderable;

class CBaseEntity
{
public:
	bool IsDormant() // ???
	{
		PVOID pNetworkable = (PVOID)(this + 0x10);
		typedef bool(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pNetworkable, 8)(pNetworkable);
	}
	int GetIndex() // ???
	{
		PVOID pNetworkable = (PVOID)(this + 0x10);
		typedef int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pNetworkable, 9)(pNetworkable);
	}
	ClientClass* GetClientClass() // ???
	{
		PVOID pNetworkable = (PVOID)(this + 0x10);
		typedef ClientClass* (__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pNetworkable, 2)(pNetworkable);
	}
	Vector& GetAbsOrigin()
	{
		typedef Vector& (__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 9)(this);
	}
	bool SetupBones(matrix3x4 *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		PVOID pRenderable = (PVOID)(this + 0x8);
		typedef bool(__thiscall* OriginalFn)(PVOID, matrix3x4*, int, int, float);
		return getvfunc<OriginalFn>(pRenderable, 13)(pRenderable, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
	const char* GetPlayerModel()
	{
		PVOID pRenderable = (PVOID)(this + 0x8);
		typedef char*(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pRenderable, 8)(pRenderable);
	}
};

struct QAngle
{
	float x;
	float y;
	float z;
};

struct CUserCmd
{
	int command_number;
	int tick_count;
	int unk001;
	QAngle viewangles;
};

class CInput
{
public:
	CUserCmd* GetUserCmd(int nSlot, int seq)
	{
		typedef CUserCmd* (__thiscall* OriginalFn)(PVOID, int, int);
		return getvfunc<OriginalFn>(this, 8)(this, nSlot, seq); // 16
	}
};

class EngineClient
{
public:
	void GetScreenSize(int& wide, int& tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int&, int&);
		return getvfunc<OriginalFn>(this, 12)(this, wide, tall);
	}
	bool GetPlayerInfo(int ent_num, player_info_t *pinfo)
	{
		typedef bool(__thiscall* OriginalFn)(PVOID, int, player_info_t *);
		return getvfunc<OriginalFn>(this, 16)(this, ent_num, pinfo);
	}
	int GetLocalPlayer(void)
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 23)(this);
	}
};

class CEntList
{
public:
	CBaseEntity* GetClientEntity(int entnum)
	{
		typedef CBaseEntity* (__thiscall* OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(this, 3)(this, entnum);
	}
	int GetHighestEntityIndex(void)
	{
		typedef int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(this, 6)(this);
	}
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 14)(this, r, g, b, a);
	}
	void DrawOutlineRectangle(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 19)(this, x0, y0, x1, y1);
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 20)(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(float font)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, float);
		getvfunc<OriginalFn>(this, 26)(this, font);
	}
	void DrawSetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int, int, int);
		getvfunc<OriginalFn>(this, 27)(this, r, g, b, a);
	}
	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int, int);
		return getvfunc<OriginalFn>(this, 28)(this, x, y);
	}
	void DrawPrintText(const wchar_t *text, int textLen)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, const wchar_t *, int, int);
		return getvfunc<OriginalFn>(this, 30)(this, text, textLen, 0);
	}
};

class IPanel
{
public:
	void GetPos(VPANEL vguiPanel, int& wide, int& tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, VPANEL, int&, int&);
		return getvfunc<OriginalFn>(this, 3)(this, vguiPanel, wide, tall);
	}
	void GetSize(VPANEL vguiPanel, int& wide, int& tall)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, VPANEL, int&, int&);
		return getvfunc<OriginalFn>(this, 5)(this, vguiPanel, wide, tall);
	}
	VPANEL GetParent(VPANEL vguiPanel)
	{
		typedef VPANEL(__thiscall* OriginalFn)(PVOID, VPANEL);
		return getvfunc<OriginalFn>(this, 26)(this, vguiPanel);
	}
	const char* GetName(VPANEL vguiPanel)
	{
		typedef const char* (__thiscall* OriginalFn)(PVOID, VPANEL);
		return getvfunc<OriginalFn>(this, 42)(this, vguiPanel);
	}
};