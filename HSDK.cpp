#include "HSDK.hpp"

DWORD
		localPlayer = 0x00,
		entityList = 0x00,
		entityLoopDist = 0x10,
		entTeam = 0x00,
		entGlowIndex = 0x1DB8,
		glowObjectBase = 0x00,
		glowObjectCount = 0x04,
		inCross = 0x00,
		attackBtn = 0x00,
		stationary = 0xE9,
		engPtr = 0x00,
		viewAechse = 0x00,
		viewOrigin = 0x00,
		vecPunch = 0x00,
		gameRes = 0x00,
		firedShots = 0x00,
		compRank = 0x00,
		compWins = 0x00,
		radar = 0x00,
		bone = 0x00,
		life = 0x00,
		vecAngle = 0x00,
		activWeap = 0x00,
		itmDI = 0x00,
		flashDur = 0x00,
		c4Time = 0x00,
		c4Tick = 0x00,
		c4Def = 0x00,
		flags = 0x00,
		lifeSt = 0x00,
		coll = 0x00,
		collGrp = 0x00,
		rgflCoordinateFrame = 0x00;

NetvarManager::Manager *netMngr;
MUtil *mUtil;
unsigned long ui_font = NULL;

DWORD MUtil::findPattern(char *pattern, char *mask, DWORD start, DWORD end, DWORD offset)
{
	int patternLength = strlen(mask);
	bool found = false;

	for (DWORD i = start; i < end - patternLength; i++)
	{
		found = true;

		for (int idx = 0; idx < patternLength; idx++)
		{
			if (*(mask + idx) == 'x' && *(pattern + idx) != *(char*)(i + idx))
			{
				found = false;
				break;
			}
		}

		if (found)
			return i + offset;
	}
	return NULL;
}

bool MUtil::findOffset(DWORD moduleBase, DWORD moduleSize, char *pattern, char *mask, DWORD addressOffset, char *offsetName, DWORD *offsetAddress, bool subtractBase)
{
	DWORD addr = this->findPattern(pattern, mask, moduleBase, moduleBase + moduleSize, addressOffset);

	if (addr)
	{
		*offsetAddress = *(DWORD*)addr;

		if (subtractBase)
			*offsetAddress -= moduleBase;

		return true;
	}
	return false;
}

void MUtil::getOffsets(DWORD clientDll, DWORD clientDllSize, DWORD engineDll, DWORD engineDllSize)
{
	bool offsetsComplete = true;

	firedShots = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_iShotsFired"));
	flashDur = netMngr->getOffs(XorStr("CCSPlayer"), XorStr("m_flFlashDuration"));
	compRank = netMngr->getOffs(XorStr("CCSPlayerResource"), XorStr("m_iCompetitiveRanking"));
	compWins = netMngr->getOffs(XorStr("CCSPlayerResource"), XorStr("m_iCompetitiveWins"));
	bone = netMngr->getOffs(XorStr("CBaseAnimating"), XorStr("m_nForceBone"));
	bone += 0x1C;
	life = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_iHealth"));
	flags = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_fFlags"));
	lifeSt = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_lifeState"));
	viewOrigin = netMngr->getOffs(XorStr("CBaseEntity"), XorStr("m_vecOrigin"));
	entTeam = netMngr->getOffs(XorStr("CBaseEntity"), XorStr("m_iTeamNum"));
	vecAngle = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_vecViewOffset[0]"));
	activWeap = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_hActiveWeapon"));
	itmDI = netMngr->getOffs(XorStr("CBaseCombatWeapon"), XorStr("m_iItemDefinitionIndex"));
	c4Time = netMngr->getOffs(XorStr("CPlantedC4"), XorStr("m_flC4Blow"));
	c4Tick = netMngr->getOffs(XorStr("CPlantedC4"), XorStr("m_bBombTicking"));
	c4Def = netMngr->getOffs(XorStr("CPlantedC4"), XorStr("m_bBombDefused"));
	coll = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_Collision"));
	collGrp = netMngr->getOffs(XorStr("CBasePlayer"), XorStr("m_CollisionGroup"));

	rgflCoordinateFrame = collGrp - 0x30;

	if (!this->findLocalPlayer(clientDll, clientDllSize))
		offsetsComplete = false;

	if (!this->findEntityList(clientDll, clientDllSize))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\xE8\x00\x00\x00\x00\x83\xC4\x04\xB8\x00\x00\x00\x00\xC3\xCC"), XorStr("x????xxxx????xx"), 9, XorStr("glowObjectBase"), &glowObjectBase, true))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\xEB\x03\x0F\x57\xC0\x8B\xB7\x00\x00\x00\x00\xF3\x0F\x11\x45\xE8\xE8"), XorStr("xxxxxxx????xxxxxx"), 7, XorStr("entGlowIndex"), &entGlowIndex))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\x89\x15\x00\x00\x00\x00\x8B\x15\x00\x00\x00\x00\xF6\xC2\x03\x74\x03\x83\xCE\x04\xA8\x04\xBF"), XorStr("xx????xx????xxxxxxxxxxx"), 2, XorStr("attackBtn"), &attackBtn, true))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\x56\x57\x8B\xF9\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00\x8B\x0D"), XorStr("xxxxxx????xx"), 6, XorStr("inCross"), &inCross))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\x0F\x7E\x82\x00\x00\x00\x00\x8B\x82\x00\x00\x00\x00\x66\x0F\xD6\x45\xD0\xF3"), XorStr("xxx????xx????xxxxxx"), 3, XorStr("vecPunch"), &vecPunch))
		offsetsComplete = false;

	if (!this->findOffset(engineDll, engineDllSize, XorStr("\xF3\x0F\x5C\xC1\xF3\x0F\x10\x15\x00\x00\x00\x00\x0F\x2F\xD0\x76\x04\xF3\x0F\x58\xC1\xA1\x00\x00\x00\x00\xF3\x0F\x11\x80\x00\x00\x00\x00\xD9\x46\x04"), XorStr("xxxxxxxx????xxxxxxxxxx????xxxx????xxx"), 22, XorStr("engPtr"), &engPtr, true))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7"), XorStr("xx????xxxx????xx"), 2, XorStr("gameRes"), &gameRes, true))
		offsetsComplete = false;

	if (!this->findOffset(clientDll, clientDllSize, XorStr("\xA1\x00\x00\x00\x00\x8B\x0C\xB0\x8B\x01\xFF\x50\x00\x46\x3B\x35\x00\x00\x00\x00\x7C\xEA\x8B\x0D"), XorStr("x????xxxxxxx?xxx????xxxx"), 1, XorStr("radar"), &radar, true))
		offsetsComplete = false;
}

bool MUtil::findLocalPlayer(DWORD clientDll, DWORD clientDllSize)
{
	DWORD addr = this->findPattern(XorStr("\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x00"), XorStr("xxx????xx????xxxxx?"), clientDll, clientDll + clientDllSize, 0);

	if (addr)
	{
		localPlayer = *(DWORD*)(addr + 3) + *(char*)(addr + 18) - clientDll;

		return true;
	}
	return false;
}

bool MUtil::findEntityList(DWORD clientDll, DWORD clientDllSize)
{
	DWORD addr = this->findPattern(XorStr("\x05\x00\x00\x00\x00\xC1\xE9\x00\x39\x48\x04"), XorStr("x????xx?xxx"), clientDll, clientDll + clientDllSize, 0);

	if (addr)
	{
		entityList = *(DWORD*)(addr + 1) + *(char*)(addr + 7) - clientDll;

		return true;
	}
	return false;
}

void MUtil::init()
{
	do
	{
		hClientDll = GetModuleHandleA(XorStr("client.dll"));
		hEngineDll = GetModuleHandleA(XorStr("engine.dll"));

	} while (hClientDll == NULL && hEngineDll == NULL);

	GetModuleInformation(GetCurrentProcess(), hClientDll, &modClientDll, sizeof(MODULEINFO));
	GetModuleInformation(GetCurrentProcess(), hEngineDll, &modEngineDll, sizeof(MODULEINFO));

	dClientDll = (DWORD)modClientDll.lpBaseOfDll;
	dEngineDll = (DWORD)modEngineDll.lpBaseOfDll;
	dClientDllSize = modClientDll.SizeOfImage;
	dEngineDllSize = modEngineDll.SizeOfImage;

	netMngr = new NetvarManager::Manager();
	netMngr->init();

	this->getOffsets(dClientDll, dClientDllSize, dEngineDll, dEngineDllSize);
}

MUtil::~MUtil()
{
	delete netMngr;
}

DWORD MUtil::getClientMode()
{
	return this->findPattern(XorStr("\x8B\x35\x00\x00\x00\x00\x85\xFF\x74\x73"), XorStr("xx????xxxx"), dClientDll, dClientDll + dClientDllSize, 2);
}

DWORD MUtil::getIsRdy()
{
	return this->findPattern(XorStr("\x55\x8B\xEC\x51\x56\x8B\x35\x00\x00\x00\x00\x80\x7E\x58\x00"), XorStr("xxxxxxx????xxxx"), dClientDll, dClientDll + dClientDllSize, 0);
}

DWORD MUtil::getGlobalVars()
{
	return this->findPattern(XorStr("\xA1\x00\x00\x00\x00\x5F\x8B\x40\x10"), XorStr("x????xxxx"), dClientDll, dClientDll + dClientDllSize, 0);
}

DWORD MUtil::getLocalPlayer()
{
	return *(DWORD*)(dClientDll + localPlayer);
}

DWORD MUtil::getEntityList()
{
	return dClientDll + entityList;
}

DWORD MUtil::getEngine()
{
	return *(DWORD*)(dEngineDll + engPtr);
}

int MUtil::getLocPlyrConnect(DWORD base)
{
	return *(int*)(base + 0x100);
}

DWORD MUtil::getEntityById(DWORD id)
{
	DWORD entityList = this->getEntityList();

	if (!entityList)
		return NULL;

	return *(DWORD*)(entityList + entityLoopDist * id);
}

DWORD MUtil::getGlowObjects()
{
	return *(DWORD*)(dClientDll + glowObjectBase);
}

DWORD MUtil::getGlowObjectCount()
{
	return *(DWORD*)(dClientDll + glowObjectBase + glowObjectCount);
}

DWORD MUtil::getInCross()
{
	DWORD locPlayer = this->getLocalPlayer();

	if (!locPlayer)
		return NULL;

	return *(DWORD*)(locPlayer + inCross);
}

BYTE MUtil::getAtk()
{
	return *(BYTE*)(dClientDll + attackBtn);
}

char *MUtil::getWeapName(int idx)
{
	switch (idx)
	{
	case weapon_none:
		return XorStr("Unknown");
	case weapon_deagle:
		return XorStr("Desert Eagle");
	case weapon_elite:
		return XorStr("Dual Berettas");
	case weapon_fiveseven:
		return XorStr("Five-SeveN");
	case weapon_glock:
		return XorStr("Glock-18");
	case weapon_ak47:
		return XorStr("AK-47");
	case weapon_aug:
		return XorStr("AUG");
	case weapon_awp:
		return XorStr("AWP");
	case weapon_famas:
		return XorStr("FAMAS");
	case weapon_g3sg1:
		return XorStr("G3SG1");
	case weapon_galilar:
		return XorStr("Galil AR");
	case weapon_m249:
		return XorStr("M249");
	case weapon_m4a1:
		return XorStr("M4A4");
	case weapon_mac10:
		return XorStr("MAC-10");
	case weapon_p90:
		return XorStr("P90");
	case weapon_ump45:
		return XorStr("UMP-45");
	case weapon_xm1014:
		return XorStr("XM1014");
	case weapon_bizon:
		return XorStr("PP-Bizon");
	case weapon_mag7:
		return XorStr("MAG-7");
	case weapon_negev:
		return XorStr("Negev");
	case weapon_sawedoff:
		return XorStr("Sawed-Off");
	case weapon_tec9:
		return XorStr("Tec-9");
	case weapon_taser:
		return XorStr("Zeus x27");
	case weapon_hkp2000:
		return XorStr("P2000");
	case weapon_mp7:
		return XorStr("MP7");
	case weapon_mp9:
		return XorStr("MP9");
	case weapon_nova:
		return XorStr("Nova");
	case weapon_p250:
		return XorStr("P250");
	case weapon_scar20:
		return XorStr("SCAR-20");
	case weapon_sg553:
		return XorStr("SG 553");
	case weapon_ssg08:
		return XorStr("SSH 08");
	case weapon_knife:
		return XorStr("Knife");
	case weapon_flashbang:
		return XorStr("Flashbang");
	case weapon_hegrenade:
		return XorStr("High Explosive Grenade");
	case weapon_smokegrenade:
		return XorStr("Smoke Grenade");
	case weapon_molotov:
		return XorStr("Molotov");
	case weapon_decoy:
		return XorStr("Decoy Grenade");
	case weapon_incgrenade:
		return XorStr("Incendiary Grenade");
	case weapon_c4:
		return XorStr("C4 Explosive");
	case weapon_knife_t:
		return XorStr("Knife");
	case weapon_m4a1_silencer:
		return XorStr("M4A1-S");
	case weapon_usp_silencer:
		return XorStr("USP-S");
	case weapon_cz75a:
		return XorStr("CZ75-Auto");
	case weapon_revolver:
		return XorStr("R8 Revolver");
	case weapon_knife_bayonet:
		return XorStr("Bayonet");
	case weapon_knife_flip:
		return XorStr("Flip Knife");
	case weapon_knife_gut:
		return XorStr("Gut Knife");
	case weapon_knife_karambit:
		return XorStr("Karambit");
	case weapon_knife_m9_bayonet:
		return XorStr("M9 Bayonet");
	case weapon_knife_tactical:
		return XorStr("Huntsman Knife");
	case weapon_knife_falchion:
		return XorStr("Falchion Knife");
	case weapon_knife_survival_bowie:
		return XorStr("Bowie Knife");
	case weapon_knife_butterfly:
		return XorStr("Butterfly Knife");
	case weapon_knife_push:
		return XorStr("Shadow Daggers");
	default:
		return XorStr("Unknown");
	}
}

bool MUtil::isWeapPistol(int idx)
{
	return(idx == weapon_deagle || idx == weapon_elite || idx == weapon_fiveseven || idx == weapon_glock
		|| idx == weapon_p250 || idx == weapon_usp_silencer || idx == weapon_tec9 || idx == weapon_taser || idx == weapon_hkp2000
		|| idx == weapon_cz75a || idx == weapon_revolver);
}

bool MUtil::isWeapSniper(int idx)
{
	return (idx == weapon_awp || idx == weapon_ssg08 || idx == weapon_scar20 || idx == weapon_g3sg1 || idx == weapon_sg553);
}

bool MUtil::isWeapNonAim(int idx)
{
	return (idx == weapon_c4 || idx == weapon_knife || idx == weapon_knife_bayonet || idx == weapon_knife_butterfly || idx == weapon_knife_falchion
		|| idx == weapon_knife_flip || idx == weapon_knife_gut || idx == weapon_knife_karambit || idx == weapon_knife_m9_bayonet || idx == weapon_knife_push
		|| idx == weapon_knife_survival_bowie || idx == weapon_knife_t || idx == weapon_knife_tactical || idx == weapon_flashbang || idx == weapon_hegrenade
		|| idx == weapon_smokegrenade || idx == weapon_molotov || idx == weapon_decoy || idx == weapon_incgrenade);
}

void MUtil::normalizeAngles(vec3_t &vecIn)
{
	if (!std::isfinite(vecIn.x))
		vecIn.x = 0;
	if (!std::isfinite(vecIn.y))
		vecIn.y = 0;

	while (vecIn.y < -180.0f)
		vecIn.y += 360.0f;
	while (vecIn.y > 180.0f)
		vecIn.y -= 360.0f;

	if (vecIn.x > 89.0f)
		vecIn.x = 89.0f;
	if (vecIn.x < -89.0f)
		vecIn.x = -89.0f;

	vecIn.z = 0;
}

float MUtil::distance(vec3_t loc, vec3_t rem)
{
	return (float)sqrt(pow(double(loc.x - rem.x), 2.0) + pow(double(loc.y - rem.y), 2.0) + pow(double(loc.z - rem.z), 2.0));
}

vec3_t MUtil::calcVector(vec3_t src, vec3_t dst)
{
	vec3_t angle;
	vec3_t delta;

	delta = src - dst;

	double hyp = sqrt(pow(delta.x, 2.0f) + pow(delta.y, 2.0f));

	angle.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
	angle.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);
	angle.z = 0.0f;

	if (delta.x >= 0.0)
		angle.y += 180.0f;

	return angle;
}

void MUtil::smooth(vec3_t src, vec3_t &dst, float factor)
{
	vec3_t delta;
	delta = dst - src;

	this->normalizeAngles(delta);

	dst = src + delta / factor;
}

void MUtil::angleVector(vec3_t src, vec3_t &dst)
{
	float p, y, t;

	p = (float)(src.x * 3.14159265358979323846f / 180);
	y = (float)(src.y * 3.14159265358979323846f / 180);
	t = (float)(cos(p));

	dst.x = (float)(-t * -cos(y));
	dst.y = (float)(sin(y) * t);
	dst.z = (float)(-sin(p));
}

float MUtil::getFoV(vec3_t angles, vec3_t src, vec3_t dst)
{
	float fov;
	vec3_t angle, target;

	angle = this->calcVector(src, dst);
	this->angleVector(angles, target);
	this->angleVector(angle, angle);

	float s = sqrt(pow(target.x, 2.0f) + pow(target.y, 2.0f) + pow(target.z, 2.0f));
	float d = sqrt(pow(target.x, 2.0f) + pow(target.y, 2.0f) + pow(target.z, 2.0f));

	float dot = (target.x*angle.x) + (target.y*angle.y) + (target.z*angle.z);

	fov = acos(dot / (s*d)) * (180.0f / 3.14159265358979323846f);
	return fov;
}

bool MUtil::isVis(vec3_t loc, vec3_t rem, Entity *base)
{
	Entity *locPlyr = reinterpret_cast<Entity*>(this->getLocalPlayer());
	if (locPlyr && locPlyr->isPlayer() && base->isPlayer() && locPlyr->getLife() > 0) // just an additional check to avoid crashing if there is no local player
	{
		trace_t tr;
		Ray_t ray;
		TraceFilterSkipTwoEntities filter(reinterpret_cast<void*>(locPlyr), reinterpret_cast<void*>(base));

		ray.Init(loc, rem);
		p_EngineTrace->TraceRay(ray, 0x4600400B, (TraceFilter*)&filter, &tr);

		if (tr.DH())
			return false;

		return tr.m_pEntityHit == reinterpret_cast<void*>((DWORD)base) || tr.IV();
	}
	return false;
}

bool MUtil::screenTransform(const vec3_t &dot, vec3_t &screen)
{
	const D3DXMATRIX w2sM = p_Engine->worldToScreenMatrix();

	screen.x = w2sM.m[0][0] * dot.x + w2sM.m[0][1] * dot.y + w2sM.m[0][2] * dot.z + w2sM.m[0][3];
	screen.y = w2sM.m[1][0] * dot.x + w2sM.m[1][1] * dot.y + w2sM.m[1][2] * dot.z + w2sM.m[1][3];
	screen.z = w2sM.m[2][0] * dot.x + w2sM.m[2][1] * dot.y + w2sM.m[2][2] * dot.z + w2sM.m[2][3];

	float w = w2sM.m[3][0] * dot.x + w2sM.m[3][1] * dot.y + w2sM.m[3][2] * dot.z + w2sM.m[3][3];

	if (screen.z < 0.1f)
		return false;

	if (w < 0.001f) {
		screen.x *= 100000;
		screen.y *= 100000;
		return true;
	}

	float invW = 1.0f / w;
	screen.x *= invW;
	screen.y *= invW;

	return false;
}

bool MUtil::w2s(const vec3_t &orig, vec3_t &screen)
{
	if (!screenTransform(orig, screen))
	{
		int width, height;

		p_Engine->getScreenSize(width, height);

		screen.x = (width / 2.0f) + (screen.x * width) / 2;
		screen.y = (height / 2.0f) - (screen.y * height) / 2;

		return true;
	}
	return false;
}

DWORD Entity::getTeam()
{
	return *(DWORD*)(this + entTeam);
}

bool Entity::isPlayer()
{
	Entity *ent = NULL;

	for (int i = 0; i < 64; i++)
	{
		ent = reinterpret_cast<Entity*>(mUtil->getEntityById(i));

		if (ent == this)
			return true;
	}
	return false;
}

DWORD Entity::getGlowIndex()
{
	return *(DWORD*)(this + entGlowIndex);
}

bool Entity::isStationary()
{
	return *(bool*)(this + stationary);
}

bool Entity::getLifeState()
{
	return *(bool*)(this + lifeSt);
}

int Entity::getFlags()
{
	return *(int*)(this + flags);
}

int Entity::getFiredShots()
{
	return *(int*)(this + firedShots);
}

vec3_t Entity::getVecPunch()
{
	return *(vec3_t*)(this + vecPunch);
}

vec3_t Entity::getVecAngles()
{
	return *(vec3_t*)(this + vecAngle);
}

vec3_t Entity::getViewOrigin()
{
	return *(vec3_t*)(this + viewOrigin);
}

int Entity::getCompRank(int idx)
{
	DWORD tmp = *(DWORD*)(mUtil->dClientDll + gameRes);
	return *(int*)((DWORD)tmp + compRank + (int)idx * 4);
}

int Entity::getCompWins(int idx)
{
	DWORD tmp = *(DWORD*)(mUtil->dClientDll + gameRes);
	return *(int*)((DWORD)tmp + compWins + (int)idx * 4);
}

std::wstring Entity::getName(int idx)
{
	DWORD tmpBase = *(DWORD*)(mUtil->dClientDll + radar);
	DWORD tmp = *(DWORD*)(tmpBase + 0x50);

	wchar_t *wcName = (wchar_t*)(tmp + 0x1E0 * idx + 0x24);
	std::wstring name(wcName);

	return name;
}

int Entity::getLife()
{
	return *(int*)(this + life);
}

vec3_t Entity::getBoneById(int idx)
{
	DWORD tmp = *(DWORD*)(this + bone);

	vec3_t tmpVekSkelet;
	tmpVekSkelet.x = *(float*)(tmp + 0x30 * idx + 0xC);
	tmpVekSkelet.y = *(float*)(tmp + 0x30 * idx + 0x1C);
	tmpVekSkelet.z = *(float*)(tmp + 0x30 * idx + 0x2C);

	return tmpVekSkelet;
}

int Entity::getWeaponId()
{
	DWORD weap = *(DWORD*)(this + activWeap);
	weap &= 0xFFF;
	DWORD plyrWeap = mUtil->getEntityById(weap - 1);

	if (plyrWeap)
		return *(int*)(plyrWeap + itmDI);
	else
		return 0;
}

Client *Entity::getClientCls()
{
	void *netwable = (void*)(this + 0x8);
	typedef Client*(__thiscall *o_GetClientCls)(void*);
	return VTManager::vfunc<o_GetClientCls>(netwable, 2)(netwable);
}

int Entity::getIdx()
{
	void *netwable = (void*)(this + 0x8);
	typedef int(__thiscall *o_GetIdx)(void*);
	return VTManager::vfunc<o_GetIdx>(netwable, 10)(netwable);
}

Collideable *Entity::getCollideable()
{
	return (Collideable*)(this + coll);
}

DWORD Entity::getRgflCoordinateFrame()
{
	return rgflCoordinateFrame;
}