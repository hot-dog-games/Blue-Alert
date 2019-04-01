#ifndef UI_BAR_H
#define UI_BAR_H

#include "UIElement.h"

class Stat;

class UIBar : public UIElement 
{
public:
	UIBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, bool is_interactable = true);
	~UIBar();

	void DecreaseBar(uint value);
	void IncreaseBar(uint value);
	bool UIBlit();
	bool Update(float dt);

	//bool CleanUp();

private:
	Stat* bar_value = nullptr;
	uint current_value = 0;

};

#endif // !UI_BAR_H
