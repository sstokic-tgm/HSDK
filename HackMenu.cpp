#include "HackMenu.hpp"

HMenu::HMenu(std::string Name, float maxwidth)
{
	title = Name;
	cur = visible = 0;
	x = y = 15.f;
	totwidth = ofs = maxwidth;
	height = 15.f;
	titleheight = totheight = height + 4.f;

	col_title = MCOLOR_TITLE;
	col_group = MCOLOR_GROUP;
	col_text = MCOLOR_TEXT;
	col_off = MCOLOR_OFF;
	col_on = MCOLOR_ON;
	col_current = MCOLOR_CURRENT;
}

void HMenu::AddItem(std::string txt, int& var, std::string opt[], int maxval, int minval, int typ)
{
	HMenuItem menu(typ, txt, opt, var, maxval, minval);

	Items.push_back(menu);

	totheight = (Items.size() *height) + titleheight;
}

void HMenu::AddGroup(std::string txt, int &var, std::string opt[], int maxval, int minval)
{
	this->AddItem(txt, var, opt, maxval, minval, MENUGROUP);
}

void HMenu::AddText(std::string txt, std::string& opt)
{
	int i;
	this->AddItem(txt, i, &opt, 0, MENUTEXT);
}

void HMenu::Show(DrawManager *drawManager)
{
	int	val;
	DWORD color;
	DWORD colorOnOf;

	if (!visible)
	{
		return;
	}

	float cy = y;

	this->DrawGui(drawManager, 0x000000);
	drawManager->drawString(ui_font, true, x + totwidth / 2, cy + 8, Color::DWORD2RGBA(col_title), title.c_str());

	cy += titleheight;

	for (unsigned int i = 0; i < Items.size(); i++)
	{
		auto &item = Items[i];
		if (item.typ == MENUTEXT)
		{
			drawManager->drawString(ui_font, false, x, cy, Color::DWORD2RGBA(col_text), item.txt.c_str());

			if (item.opt)
				drawManager->drawStringRight(ui_font, true, x + ofs, cy, Color::DWORD2RGBA(col_text), item.opt[0].c_str());

		}
		else
		{
			val = (item.var) ? (item.var) : 0;

			if (i == cur)
				color = col_current;
			else if (item.typ == MENUGROUP)
				color = col_group;
			else
				color = col_text;

			drawManager->drawString(ui_font, false, x, cy, Color::DWORD2RGBA(color), item.txt.c_str());

			if (item.opt)
			{
				drawManager->drawStringRight(ui_font, true, x + ofs, cy, Color::DWORD2RGBA((val) ? col_on : col_off), item.opt[val].c_str());
			}
		}
		cy += height;
	}
}

void HMenu::DrawGui(DrawManager *drawManager, DWORD col)
{
	if (background == 5)					// solid
		col |= 0xff000000;
	else
		col |= background * 0x30000000;	// transparency

	drawManager->drawFilledRect(x - 3.f, y - 2.f, totwidth + 6.f, totheight + 4.f, Color::DWORD2RGBA(col));
	drawManager->drawFilledRect(x - 3.f, y - 2.f, totwidth + 6.f, titleheight + 1.f, Color::DWORD2RGBA(col));
}

void HMenu::Nav(void)
{
	if (GetAsyncKeyState(VK_NUMPAD5) & 1)
		visible = !visible;

	if (!visible)
		return;

	if (GetAsyncKeyState(VK_NUMPAD0))
	{
		if (GetAsyncKeyState(VK_UP) & 1) y -= 10;
		if (GetAsyncKeyState(VK_DOWN) & 1) y += 10;
		if (GetAsyncKeyState(VK_LEFT) & 1) x -= 10;
		if (GetAsyncKeyState(VK_RIGHT) & 1) x += 10;
	}
	else
	{
		if (GetAsyncKeyState(VK_UP) & 1)
		{
			do {
				cur--;
				if (cur<0)
					cur = Items.size() - 1;
			} while (Items[cur].typ == MENUTEXT);
		}
		else if (GetAsyncKeyState(VK_DOWN) & 1)
		{
			do {
				cur++;
				if (cur == Items.size()) cur = 0;
			} while (Items[cur].typ == MENUTEXT);
		}
		else
		{
			int dir = 0;

			if (GetAsyncKeyState(VK_LEFT) & 1 && Items[cur].var > (Items[cur].minval)) dir = -1;
			if (GetAsyncKeyState(VK_RIGHT) & 1 && Items[cur].var < (Items[cur].maxval - 1)) dir = 1;

			if (dir)
			{
				Items[cur].var += dir;
				if (Items[cur].typ == MENUGROUP)
				{
					Items.clear();
				}
			}
		}
	}
}