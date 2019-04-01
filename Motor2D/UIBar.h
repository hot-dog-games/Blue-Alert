#ifndef UI_BAR_H
#define UI_BAR_H

#include "UIElement.h"

class UIBar : public UIElement 
{
public:
	UIBar(iPoint pos, SDL_Rect sprite_rect, uint max, bool is_interactable = true);
	~UIBar();

	void DecreaseBar(uint value);
	void IncreaseBar();
	bool UIBlit();

	//bool CleanUp();

private:
	uint max_value = 0;

};

#endif // !UI_BAR_H
