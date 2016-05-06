#pragma once

#include <iostream>
#include <Windows.h>
#include <Psapi.h>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <random>

#include "XorStr.hpp"

#include "InterfaceManager.hpp"
#include "NetvarManager.hpp"
#include "VTManager.hpp"
#include "VMTManager.hpp"

#include "GlowObjectDefinition.hpp"
#include "Color.hpp"
#include "Util_Vector.hpp"

#include "ClientMode.hpp"
#include "EngineClient.hpp"
#include "GlobalVars.hpp"
#include "Input.hpp"
#include "Panel.hpp"
#include "Surface.hpp"
#include "EngineTrace.hpp"
#include "Console.hpp"

#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "psapi.lib")

extern DWORD
		localPlayer,
		entityList,
		entityLoopDist,
		entTeam,
		entGlowIndex,
		glowObjectBase,
		glowObjectCount,
		inCross,
		attackBtn,
		stationary,
		engPtr,
		viewAechse,
		viewOriginal,
		vekPunch,
		gameRes,
		metakFired,
		compRank,
		compWins,
		radar,
		skelet,
		leben,
		vecAechse,
		activWeap,
		itmDI,
		coravDur,
		c4Time,
		c4Tick,
		c4Def,
		flags,
		lifeSt,
		coll,
		collGrp,
		rgflCoordinateFrame;

class Entity;
class MUtil;
class Collideable;

class Entity
{

public:

	DWORD getTeam();
	bool isPlayer();
	DWORD getGlowIndex();
	bool isStationary();
	bool getLifeState();
	int getFlags();
	int getFiredShots();
	vec3_t getVecPunch();
	vec3_t getVecAngles();
	vec3_t getViewOrigin();
	int getCompRank(int idx);
	int getCompWins(int idx);
	std::wstring getName(int idx);
	int getLife();
	vec3_t getBoneById(int idx);
	int getWeaponId();
	Client *getClientCls();
	int getIdx();
	Collideable *getCollideable();
	DWORD getRgflCoordinateFrame();
};

class Collideable
{
public:

	virtual void unk0();
	virtual const vec3_t &vecMins() const;
	virtual const vec3_t &vecMax() const;
};

class MUtil
{
public:

	HMODULE		hClientDll, hEngineDll;
	DWORD_PTR	clientDllSize, engineDllSize;
	MODULEINFO	modClientDll, modEngineDll;
	DWORD		dClientDll, dEngineDll, dClientDllSize, dEngineDllSize;

	void init();
	MUtil() {};
	~MUtil();

	DWORD getClientMode();
	DWORD getIsRdy();
	DWORD getGlobalVars();
	DWORD getEntityList();
	DWORD getLocalPlayer();
	DWORD getEngine();
	int getLocPlyrConnect(DWORD base);
	DWORD getEntityById(DWORD id);
	DWORD getGlowObjects();
	DWORD getGlowObjectCount();
	DWORD getInCross();
	BYTE getAtk();

	char *getWeapName(int idx);
	bool isWeapPistol(int idx);
	bool isWeapSniper(int idx);
	bool isWeapNonAim(int idx);

	void normalizeAngles(vec3_t &vektRein);
	float distance(vec3_t loc, vec3_t rem);
	vec3_t calcVector(vec3_t src, vec3_t dst);
	void smooth(vec3_t src, vec3_t &dst, float factor);
	void angleVector(vec3_t src, vec3_t &dst);
	float getFoV(vec3_t angles, vec3_t src, vec3_t dst);
	bool isVis(vec3_t loc, vec3_t rem, Entity *base);

	bool screenTransform(const vec3_t &dot, vec3_t &screen);
	bool w2s(const vec3_t &orig, vec3_t &screen);

private:

	DWORD findPattern(char *pattern, char *mask, DWORD start, DWORD end, DWORD offset);
	bool findOffset(DWORD moduleBase, DWORD moduleSize, char *pattern, char *mask, DWORD addressOffset, char *offsetName, DWORD *offsetAddress, bool subtractBase = false);
	void getOffsets(DWORD clientDll, DWORD clientDllSize, DWORD engineDll, DWORD engineDllSize);
	bool findLocalPlayer(DWORD clientDll, DWORD clientDllSize);
	bool findEntityList(DWORD clientDll, DWORD clientDllSize);
};

static const wchar_t *ranks[] =
{
	L"-",
	L"Silver I",
	L"Silver II",
	L"Silver III",
	L"Silver IV",
	L"Silver Elite",
	L"Silver Elite Master",

	L"Gold Nova I",
	L"Gold Nova II",
	L"Gold Nova III",
	L"Gold Nova Master",
	L"Master Guardian I",
	L"Master Guardian II",

	L"Master Guardian Elite",
	L"Distinguished Master Guardian",
	L"Legendary Eagle",
	L"Legendary Eagle Master",
	L"Supreme Master First Class",
	L"The Global Elite"
};

enum CSGO_Weapon_ID
{
	weapon_none = 0,
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_ump45 = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg553 = 39,
	weapon_ssg08 = 40,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_knife_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_push = 516
};

extern MUtil *mUtil;
extern unsigned long ui_font;

void RebuildMenu();
DWORD __stdcall InitThread(LPVOID unused);