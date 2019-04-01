#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIBar.h"
#include "Summoner.h"

UIBar::UIBar(iPoint pos, SDL_Rect sprite_rect, uint* current, uint max, bool is_interactable)
{
	interactable = is_interactable;
	rect_box = { pos.x, pos.y, sprite_rect.w,sprite_rect.h };

	rect_sprite = sprite_rect;
	max_value = max;
	current_value = max_value;
	current_extern_value = current;
}

void UIBar::DecreaseBar(uint value)
{
	uint height = (rect_box.h / max_value) * value;
	rect_sprite.h -= height;
	rect_box.y += height;
	current_value -= value;
}

void UIBar::IncreaseBar(uint value)
{
	uint height = (rect_box.h / max_value) * value;
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
	if (*current_extern_value > current_value) {
		IncreaseBar(*current_extern_value - current_value);
	}
	else if (*current_extern_value < current_value) {
		DecreaseBar(current_value - *current_extern_value);
	}
		

	return true;
}

uint UIBar::GetMaxValue() const
{
	return max_value;
}

uint UIBar::GetCurrentValue() const
{
	return current_value;
}
