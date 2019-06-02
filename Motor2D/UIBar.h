#ifndef UI_BAR_H
#define UI_BAR_H

#include "UIElement.h"

class Stat;

enum BarType;
enum BarState;

class UIBar : public UIElement 
{
public:
	UIBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, BarType type, BarState state);
	~UIBar() {};

	void DecreaseBar(float value);
	void IncreaseBar(float value);
	bool UIBlit();
	bool Update(float dt);

	void ChangeStat(Stat* stat);

	int GetValue() const;

protected:
	Stat*	bar_value = nullptr;
	float	current_value = 0;
	BarType	bar_type;
	BarState bar_state;

	float decimal_decrease = 0.0f;
	float decimal_increase = 0.0f;

};

#endif // !UI_BAR_H
