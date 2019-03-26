#ifndef UI_BAR_H
#define UI_BAR_H

#include "UIElement.h"

class UIBar : public UIElement 
{
public:
	UIBar(iPoint pos, SDL_Rect sprite_rect, bool is_interactable = true);
	~UIBar();

	void LossPoint();
	void GainPoint();
	bool UIBlit();

	//bool CleanUp();

};

#endif // !UI_BAR_H
