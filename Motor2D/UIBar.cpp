#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIBar.h"
#include "Stat.h"

UIBar::UIBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, bool is_interactable)
{
	interactable = is_interactable;
	rect_box = { pos.x, pos.y, sprite_rect.w,sprite_rect.h };

	rect_sprite = sprite_rect;
	bar_value = value;
	current_value = value->GetMaxValue();
}

void UIBar::DecreaseBar(uint value)
{
	uint height = (rect_box.h / bar_value->GetMaxValue()) * value;
	rect_sprite.h -= height;
	rect_box.y += height;
	current_value -= value;
}

void UIBar::IncreaseBar(uint value)
{
	uint height = (rect_box.h / bar_value->GetMaxValue()) * value;
	rect_sprite.h += height;
	rect_box.y -= height;
	current_value += value;
}

bool UIBar::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	if (clipping && parent)
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, &parent->GetScreenRect());
	else
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);
	return true;
}

bool UIBar::Update(float dt)
{
	if (bar_value->GetValue() > current_value) {
		IncreaseBar(bar_value->GetValue() - current_value);
	}
	else if (bar_value->GetValue() < current_value) {
		DecreaseBar(current_value - bar_value->GetValue());
	}
		

	return true;
}