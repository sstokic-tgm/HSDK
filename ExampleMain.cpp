#include "HSDK.hpp"
#include "DrawManager.hpp"
#include "HackMenu.hpp"

/* Important !!! ----- start */
BaseClientDll *p_Client = nullptr;
ClientMode *p_ClientMode = nullptr;
EngineClient *p_Engine = nullptr;
Panel *p_Panel = nullptr;
Surface *p_Surface = nullptr;
EngineTrace *p_EngineTrace = nullptr;
Console *p_Console = nullptr;
GlobalVars *p_GlobalVars = nullptr;
Input *p_Input = nullptr;

DrawManager *drawManager = nullptr;
HMenu *p_HMenu = nullptr;
/* Important !!! ----- end */

std::unique_ptr<VFTableHook> g_pPaintTraverseHook = nullptr;
std::unique_ptr<VFTableHook> g_pCreateMoveHook = nullptr;

GlowObjectDefinition *glowObjects = nullptr;

std::shared_ptr<HMenu> hmenu;

// group states
int general = 1;
int visual = 1;
// item  states
int background = 0;
std::string opt_Grp[] = { "+", "-" };
std::string opt_OffOn[] = { "Off", "On" };
std::string opt_Back[] = { "Off", "20%", "40%", "60%", "80%", "Solid" };

int bEnableGlow = 0;

// Thread for ESP
void ESPThread(MUtil *mUtil, GlowObjectDefinition *glowObjects);

/* PaintTraverse defs */
typedef void(__thiscall *PaintTraverse_t)(void*, unsigned int, bool, bool);
PaintTraverse_t o_PaintTraverse = nullptr;
void __fastcall Hooked_PaintTraverse(void* thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce);

/* CreateMove defs */
typedef bool(__thiscall *CreateMove_t)(ClientMode*, float, UserCMD*);
CreateMove_t o_CreateMove = nullptr;
bool __stdcall Hooked_CreateMove(float smt, UserCMD *userCMD);

void __fastcall Hooked_PaintTraverse(void* thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	o_PaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);

	static DWORD MatSystemTopPanel;

	if (!MatSystemTopPanel)
	{
		const char* szName = p_Panel->getName(vguiPanel);

		if (lstrcmpA(szName, "MatSystemTopPanel") == 0)
		{
			MatSystemTopPanel = vguiPanel;

			ui_font = p_Surface->fontCreate();
			p_Surface->setFontGlyphSet(ui_font, "Tahoma", 14, 350, 0, 0, FONTFLAG_OUTLINE);

			p_Engine->execClientCMD("clear");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), " __  __  ____    ____    __  __       ___                        __             __  __     \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "/\\ \\/\\ \\/\\  _`\\ /\\  _`\\ /\\ \\/\\ \\     /\\_ \\                      /\\ \\           /\\ \\/\\ \\    \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "\\ \\ \\_\\ \\ \\,\\L\\_\\ \\ \\/\\ \\ \\ \\/'/'    \\//\\ \\     ___      __     \\_\\ \\     __   \\_\\ \\ \\ \\   \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), " \\ \\  _  \\/_\\__ \\\\ \\ \\ \\ \\ \\ , <       \\ \\ \\   / __`\\  /'__`\\   /'_` \\  /'__`\\ /'_` \\ \\ \\  \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "  \\ \\ \\ \\ \\/\\ \\L\\ \\ \\ \\_\\ \\ \\ \\\\`\\      \\_\\ \\_/\\ \\L\\ \\/\\ \\L\\.\\_/\\ \\L\\ \\/\\  __//\\ \\L\\ \\ \\_\\ \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "   \\ \\_\\ \\_\\ `\\____\\ \\____/\\ \\_\\ \\_\\    /\\____\\ \\____/\\ \\__/.\\_\\ \\___,_\\ \\____\\ \\___,_\\/\\_\\ \n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "    \\/_/\\/_/\\/_____/\\/___/  \\/_/\\/_/    \\/____/\\/___/  \\/__/\\/_/\\/__,_ /\\/____/\\/__,_ /\\/_/\n");
			p_Console->ConsoleColorPrintf(Color(0, 255, 0, 255), "\n");

			p_Engine->execClientCMD("toggleconsole");
		}
	}

	if (MatSystemTopPanel == vguiPanel)
	{
		if (p_HMenu->Items.size() == 0)
			RebuildMenu();

		p_HMenu->Show(drawManager);
		p_HMenu->Nav();


		if (p_Engine->isConn() && p_Engine->isIG())
		{
			ESPThread(mUtil, glowObjects);
		}
	}
}

bool __stdcall Hooked_CreateMove(float smt, UserCMD *userCMD)
{
	if (!userCMD->command_number)
		return true;

	if (p_Engine->isConn() && p_Engine->isIG())
	{
		// CreateMove stuff
	}

	return true; // if using p_Engine->setViewAngles then return false!!!
}

void RebuildMenu()
{
	p_HMenu->AddGroup(XorStr("General"), general, opt_Grp);
	if (general)
	{
		p_HMenu->AddItem(XorStr("Background"), background, opt_Back, 6);
	}

	p_HMenu->AddGroup(XorStr("ESP"), visual, opt_Grp);
	if (visual)
	{
		p_HMenu->AddItem(XorStr("Glow"), bEnableGlow, opt_OffOn);
	}
}

BOOL __stdcall DllMain(HINSTANCE hmod, DWORD dwReason, LPVOID res)
{
	DisableThreadLibraryCalls(hmod);

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:

		CreateThread(0, 0, &InitThread, 0, 0, 0);

		break;

	case DLL_PROCESS_DETACH:

		delete mUtil;
		glowObjects = nullptr;

		g_pPaintTraverseHook->RestoreTable();
		g_pCreateMoveHook->RestoreTable();

		break;
	}
	return TRUE;
}

DWORD __stdcall InitThread(LPVOID unused)
{
	hmenu = std::make_shared<HMenu>(HMenu(XorStr("HSDK"), 180));
	hmenu->visible = true;

	p_HMenu = hmenu.get();

	while (!GetModuleHandleA(XorStr("client.dll")) || !GetModuleHandleA(XorStr("engine.dll")) || !GetModuleHandleA(XorStr("vstdlib.dll")))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	p_Client = InterfaceManager::Manager::getIface<BaseClientDll>(XorStr("client.dll"), XorStr("VClient"));
	p_Engine = InterfaceManager::Manager::getIface<EngineClient>(XorStr("engine.dll"), XorStr("VEngineClient"));
	p_Panel = InterfaceManager::Manager::getIface<Panel>(XorStr("vgui2.dll"), XorStr("VGUI_Panel"));
	p_Surface = InterfaceManager::Manager::getIface<Surface>(XorStr("vguimatsurface.dll"), XorStr("VGUI_Surface"));
	p_EngineTrace = InterfaceManager::Manager::getIface<EngineTrace>(XorStr("engine.dll"), XorStr("EngineTraceClient"));
	p_Console = InterfaceManager::Manager::getIface<Console>(XorStr("vstdlib.dll"), XorStr("VEngineCvar"));

	mUtil = new MUtil();
	mUtil->init();

	p_GlobalVars = *(GlobalVars**)(*(DWORD*)(mUtil->getGlobalVars() + 0x1));
	p_Input = *(Input**)((*(DWORD**)p_Client)[15] + 0x1);

	do
	{
		p_ClientMode = *(ClientMode**)(*(DWORD*)mUtil->getClientMode());
	} while (!p_ClientMode && p_ClientMode == NULL);

	g_pPaintTraverseHook = std::make_unique<VFTableHook>((PPDWORD)p_Panel, true);
	g_pCreateMoveHook = std::make_unique<VFTableHook>((PPDWORD)p_ClientMode, true);

	o_PaintTraverse = g_pPaintTraverseHook->Hook(41, (PaintTraverse_t)Hooked_PaintTraverse);
	o_CreateMove = g_pCreateMoveHook->Hook(24, (CreateMove_t)Hooked_CreateMove);

	return EXIT_SUCCESS;
}

void ESPThread(MUtil *mUtil, GlowObjectDefinition *glowObjects)
{
	if (mUtil->getLocPlyrConnect(mUtil->getEngine()) == 6)
	{
		Entity *locPlyr = reinterpret_cast<Entity*>(mUtil->getLocalPlayer());

		if (!locPlyr)
			return;

		DWORD glowBase = mUtil->getGlowObjects();
		if (glowBase)
		{
			glowObjects = (GlowObjectDefinition*)glowBase;
		}

		for (int i = 0; i < 64; i++)
		{
			Entity *player = reinterpret_cast<Entity*>(mUtil->getEntityById(i));

			if (!player)
				continue;

			if (!player->isPlayer())
				continue;

			if (player == locPlyr)
				continue;

			if (player->isStationary())
				continue;

			if (player->getLife() <= 0)
				continue;

			if (bEnableGlow)
			{
				DWORD glowIndex = player->getGlowIndex();

				if (glowIndex >= 0 && glowIndex < mUtil->getGlowObjectCount())
				{
					// some vis checks
					vec3_t localVecAngles = locPlyr->getVecAngles();
					vec3_t localViewOrigin = locPlyr->getViewOrigin();
					vec3_t newVec;
					newVec = localViewOrigin + localVecAngles;

					bool playerTeam = player->getTeam() == 2;

					vec3_t origin = player->getViewOrigin();
					vec3_t head = player->getBoneById(6);

					bool vis = mUtil->isVis(newVec, head, player);
					float r = 0.0f, g = 0.0f, b = 0.0f;

					if (vis)
					{
						if (playerTeam)
							r = 0.9f;
						else
							b = 0.9f;
					}
					else
					{
						if (playerTeam)
						{
							r = 0.9f;
							g = 0.627f;
							b = 0.196f;
						}
						else
						{
							r = 0.196f;
							g = 0.627f;
							b = 0.9f;
						}
					}

					(*(glowObjects + glowIndex)).a = 0.9f;
					(*(glowObjects + glowIndex)).r = r;
					(*(glowObjects + glowIndex)).g = g;
					(*(glowObjects + glowIndex)).b = b;

					(*(glowObjects + glowIndex)).m_bRenderWhenOccluded = true;
					(*(glowObjects + glowIndex)).m_bRenderWhenUnoccluded = false;
				}
			}
		}
	}
}