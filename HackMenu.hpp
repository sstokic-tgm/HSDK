#pragma once

#include "DrawManager.hpp"

#define MENUGROUP 1
#define MENUTEXT 2
#define MENUITEM 3

#define MCOLOR_TITLE 0xff004c94
#define MCOLOR_CURRENT 0xffa0a0a0
#define MCOLOR_GROUP 0xff004c94
#define MCOLOR_TEXT 0xff33C8F3
#define MCOLOR_OFF 0xffff0000
#define MCOLOR_ON 0xff00ff00

struct HMenuItem {
	int typ;
	std::string txt;
	std::string* opt;
	int &var;
	int maxval;
	int minval;
	HMenuItem(int _typ, const std::string _txt, std::string _opt[], int& _val, int _maxval, int _minval)
		: typ(_typ), txt(_txt), opt(_opt), var(_val), maxval(_maxval), minval(_minval) {}
};

class HMenu
{
public:
	HMenu(std::string Name = 0, float maxwidth = 180);

	virtual ~HMenu() { }

	DWORD col_title;
	DWORD col_group;
	DWORD col_text;
	DWORD col_off;
	DWORD col_on;
	DWORD col_current;

	float x, y;
	float totwidth, totheight;
	float height;
	float titleheight;
	float ofs;

	std::string	title;
	int cur;
	bool visible;

	std::vector<HMenuItem> Items;

	void AddItem(std::string txt, int &var, std::string opt[], int maxvalue = 2, int minval = 0, int typ = MENUITEM);
	void AddGroup(std::string txt, int &var, std::string opt[], int maxvalue = 2, int minval = 0);
	void AddText(std::string txt, std::string& opt);

	void DrawGui(DrawManager *drawManager, DWORD col);

	void Show(DrawManager *drawManager);
	void Nav(void);
};

extern int background;
extern HMenu *p_HMenu;