#include "DrawManager.hpp"

void DrawManager::textW(bool center, int font, int x, int y, Color c, wchar_t *pszString)
{
	if (center)
	{
		int wide, tall;
		p_Surface->getTextSize(font, pszString, wide, tall);
		x -= wide / 2;
		y -= tall / 2;
	}
	p_Surface->drawSetTextColor(c);
	p_Surface->drawSetTextFont(font);
	p_Surface->drawSetTextPos(x, y);
	p_Surface->drawPrintText(pszString, (int)wcslen(pszString), FONT_DRAW_DEFAULT);
}

void DrawManager::drawString(int font, bool bCenter, int x, int y, Color c, const char *fmt, ...)
{
	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(fmt) + 1) * sizeof(wchar_t)));

	mbstowcs(pszStringWide, fmt, (strlen(fmt) + 1) * sizeof(wchar_t));

	textW(bCenter, font, x, y, c, pszStringWide);

	free(pszStringWide);
}

void DrawManager::textWRight(bool right, int font, int x, int y, Color c, wchar_t *pszString)
{
	if (right)
	{
		int wide, tall;
		p_Surface->getTextSize(font, pszString, wide, tall);
		x -= wide;
		//y -= tall;
	}
	p_Surface->drawSetTextColor(c);
	p_Surface->drawSetTextFont(font);
	p_Surface->drawSetTextPos(x, y);
	p_Surface->drawPrintText(pszString, (int)wcslen(pszString), FONT_DRAW_DEFAULT);
}

void DrawManager::drawStringRight(int font, bool bRight, int x, int y, Color c, const char *fmt, ...)
{
	wchar_t *pszStringWide = reinterpret_cast< wchar_t* >(malloc((strlen(fmt) + 1) * sizeof(wchar_t)));

	mbstowcs(pszStringWide, fmt, (strlen(fmt) + 1) * sizeof(wchar_t));

	textWRight(bRight, font, x, y, c, pszStringWide);

	free(pszStringWide);
}

void DrawManager::drawOutlinedRect(int x, int y, int w, int h, Color &c)
{
	p_Surface->drawSetColor(c);
	p_Surface->drawOutlinedRect(x, y, x + w, y + h);
}

void DrawManager::drawFilledRect(int x, int y, int w, int h, Color &c)
{
	p_Surface->drawSetColor(c);
	p_Surface->drawFilledRect(x, y, x + w, y + h);
}

void DrawManager::drawLine(int x, int y, int x1, int y1, Color c)
{
	p_Surface->drawSetColor(c);
	p_Surface->drawLine(x, y, x1, y1);
}

void DrawManager::fillRGBA(int x, int y, int w, int h, Color c)
{
	p_Surface->drawSetColor(c);
	p_Surface->drawFilledRect(x, y, x + w, y + h);
}

void DrawManager::drawHealthBar(int x, int y, float health, int w, int h, Color c)
{
	x -= w / 2;
	y -= h / 2;
	fillRGBA(x, y, w, h + 1, Color(0, 0, 0, 255));
	unsigned int hw = (unsigned int)(((w - 2) * health) / 100);
	fillRGBA(x + 1, y + 1, hw, h - 1, c);
}

void DrawManager::drawPlyrBox(Entity *base, MUtil *mUtil)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(mUtil->getLocalPlayer());
	if (!locPlyr)
		return;

	vec3_t localVecAngles = locPlyr->getVecAngles();
	vec3_t localViewOrigin = locPlyr->getViewOrigin();
	vec3_t newVec;
	newVec = localViewOrigin + localVecAngles;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	vec3_t origin = player->getViewOrigin();
	vec3_t head = player->getBoneById(6);

	bool vis = mUtil->isVis(newVec, head, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	head.z += 15.0f;

	vec3_t screenOrigin, screenHead;

	if (mUtil->w2s(head, screenHead) && mUtil->w2s(origin, screenOrigin))
	{
		float height = abs(screenOrigin.y - screenHead.y);
		float width = height / 2.0f;

		int leben = player->getLife();

		drawManager->drawHealthBar(screenHead.x, screenOrigin.y + 5, leben, width, 5, Color((255.f - float(leben) * 2.55f), (float(leben) * 2.55f), 0, 180));

		drawManager->drawOutlinedRect(screenHead.x - width / 2, screenHead.y, width, height, c);
	}
}

void DrawManager::drawPlyrName(int idx, Entity *base, MUtil *mUtil)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(mUtil->getLocalPlayer());
	if (!locPlyr)
		return;

	vec3_t localVecAngles = locPlyr->getVecAngles();
	vec3_t localViewOrigin = locPlyr->getViewOrigin();
	vec3_t newVec;
	newVec = localViewOrigin + localVecAngles;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	vec3_t origin = player->getViewOrigin();
	vec3_t head = player->getBoneById(6);

	bool vis = mUtil->isVis(newVec, head, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	head.z += 15.0f;

	vec3_t screenOrigin, screenHead;

	if (mUtil->w2s(head, screenHead) && mUtil->w2s(origin, screenOrigin))
	{
		wchar_t plyrName[128];
		swprintf(plyrName, L"%ls", player->getName(idx).c_str());

		drawManager->textW(true, ui_font, screenHead.x, screenHead.y - 25, c, plyrName);
	}
}

void DrawManager::drawPlyrWeapName(Entity *base, MUtil *mUtil)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(mUtil->getLocalPlayer());
	if (!locPlyr)
		return;

	vec3_t localVecAngles = locPlyr->getVecAngles();
	vec3_t localViewOrigin = locPlyr->getViewOrigin();
	vec3_t newVec;
	newVec = localViewOrigin + localVecAngles;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	vec3_t origin = player->getViewOrigin();
	vec3_t head = player->getBoneById(6);

	bool vis = mUtil->isVis(newVec, head, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	head.z += 15.0f;

	vec3_t screenOrigin, screenHead;

	if (mUtil->w2s(head, screenHead) && mUtil->w2s(origin, screenOrigin))
	{
		float height = abs(screenOrigin.y - screenHead.y);
		float width = height / 2.0f;

		drawManager->drawString(ui_font, true, screenHead.x, screenHead.y + 7, c, mUtil->getWeapName(player->getWeaponId()));
	}
}

void DrawManager::drawPlyrMMinfo(int idx, Entity *base, MUtil *mUtil)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(mUtil->getLocalPlayer());
	if (!locPlyr)
		return;

	vec3_t localVecAngles = locPlyr->getVecAngles();
	vec3_t localViewOrigin = locPlyr->getViewOrigin();
	vec3_t newVec;
	newVec = localViewOrigin + localVecAngles;

	bool playerTeam = player->getTeam() == 2;

	Color c;

	vec3_t origin = player->getViewOrigin();
	vec3_t head = player->getBoneById(6);

	bool vis = mUtil->isVis(newVec, head, player);

	if (vis)
		c = playerTeam ? Color(255, 0, 0, 255) : Color(0, 0, 255, 255);
	else
		c = playerTeam ? Color(255, 160, 50, 255) : Color(50, 160, 255, 255);

	head.z += 15.0f;

	vec3_t screenOrigin, screenHead;

	if (mUtil->w2s(head, screenHead) && mUtil->w2s(origin, screenOrigin))
	{
		int leben = player->getLife();

		char cCompRank[128];
		char cCompWins[128];
		sprintf(cCompRank, "Rank: %ls", ranks[player->getCompRank(idx)]);
		sprintf(cCompWins, "Wins: %d", player->getCompWins(idx));

		drawManager->drawString(ui_font, true, screenHead.x, screenHead.y - 5, c, cCompWins);
		drawManager->drawString(ui_font, true, screenHead.x, screenHead.y - 15, c, cCompRank);
	}
}

void DrawManager::drawAimSpot(Entity *base, int skelId, MUtil *mUtil)
{
	Entity *player = base;
	Entity *locPlyr = reinterpret_cast<Entity*>(mUtil->getLocalPlayer());
	if (!locPlyr)
		return;

	vec3_t spot = player->getBoneById(skelId);

	vec3_t screenSpot;

	if (mUtil->w2s(spot, screenSpot))
	{
		if (player->getTeam() != locPlyr->getTeam())
			drawManager->fillRGBA(screenSpot.x, screenSpot.y, 3, 3, Color(255, 255, 255, 255));
	}
}