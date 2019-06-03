#ifndef UI_BAR_ELEMENT_H
#define UI_BAR_ELEMENT_H

#include "UIBar.h"

class Stat;
class Entity;

enum BarType;

class UIEntityBar : public UIBar 
{
public:
	UIEntityBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, BarType type, BarState state, Entity* entity);
	~UIEntityBar();

	bool Update(float dt);
	bool UIBlit();

private:
	SDL_Texture* bar_texture = nullptr;
	void ChangeColor();
	Entity* entity = nullptr;
	int entity_height = 0;
	int bar_margin = 15;
};

#endif // !UI_BAR_ELEMENT_H
