#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIEntityBar.h"
#include "Stat.h"
#include "p2Log.h"
#include "Entity.h"

UIEntityBar::UIEntityBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, BarType type, BarState state, Entity* entity): UIBar(pos,sprite_rect,value,type, state)
{
	this->entity = entity;
}

bool UIEntityBar::Update(float dt)
{
	if (bar_value->GetValue() > current_value) {
		IncreaseBar(bar_value->GetValue() - current_value);
	}
	else if (bar_value->GetValue() < current_value) {
		DecreaseBar(current_value - bar_value->GetValue());
	}

	iPoint entity_pos = App->render->WorldToScreen((int)entity->position.x, (int)entity->position.y);
	rect_box.x = entity_pos.x - rect_box.w * 0.5;
	rect_box.y = entity_pos.y - entity->current_frame.h;

	return true;
}