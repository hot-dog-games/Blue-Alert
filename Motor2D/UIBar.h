#ifndef UI_BAR_H
#define UI_BAR_H

#include "UIElement.h"

class Stat;

enum BarType;

class UIBar : public UIElement 
{
public:
	UIBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, BarType type);
	~UIBar() {};

	void DecreaseBar(uint value);
	void IncreaseBar(uint value);
	bool UIBlit();
	bool Update(float dt);

protected:
	Stat*	bar_value = nullptr;
	uint	current_value = 0;
	BarType	bar_type;

	float decimal_decrease = 0.0f;
	float decimal_increase = 0.0f;

};

#endif // !UI_BAR_H
