#pragma once

#include "HSDK.hpp"

class DrawManager
{
public:

	void textW(bool center, int font, int x, int y, Color c, wchar_t *pszString);
	void drawString(int font, bool bCenter, int x, int y, Color c, const char *fmt, ...);
	void textWRight(bool right, int font, int x, int y, Color c, wchar_t *pszString);
	void drawStringRight(int font, bool right, int x, int y, Color c, const char *fmt, ...);

	void drawOutlinedRect(int x, int y, int w, int h, Color &c);
	void drawFilledRect(int x, int y, int w, int h, Color &c);
	void drawHealthBar(int x, int y, float health, int w, int h, Color c);
	virtual void drawFlashedBar(int x, int y, float time, int w, int h, Color c);
	void drawLine(int x, int y, int x1, int y2, Color c);
	void fillRGBA(int x, int y, int w, int h, Color c);

	void drawPlyrBox(Entity *base, MUtil *mUtil);
	void drawPlyrName(int idx, Entity *base, MUtil *mUtil);
	void drawPlyrWeapName(Entity *base, MUtil *mUtil);
	void drawPlyrMMinfo(int idx, Entity *base, MUtil *mUtil);
	virtual void drawPlyrFlash(Entity *base, MUtil *mUtil);
	virtual void drawC4Planted(Entity *base, MUtil *mUtil);
	void drawAimSpot(Entity *base, int skelId, MUtil *mUtil);
	virtual void drawSniperCrosshair();
	virtual void drawC4Droped(Entity *base, MUtil *mUtil);
	virtual void drawSnapLine(Entity *base, MUtil *mUtil);
};

extern DrawManager *drawManager;