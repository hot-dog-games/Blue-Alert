#ifndef UI_BAR_ELEMENT_H
#define UI_BAR_ELEMENT_H

#include "UIBar.h"

class Stat;
class Entity;

enum BarType;

class UIEntityBar : public UIBar 
{
public:
	UIEntityBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, BarType type, Entity* entity);
	~UIEntityBar();

	bool Update(float dt);

private:
	Entity* entity = nullptr;
};

#endif // !UI_BAR_ELEMENT_H
