#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIBar.h"
#include "Stat.h"
#include "p2Log.h"

UIBar::UIBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, BarType type, bool is_interactable)
{
	interactable = is_interactable;
	rect_box = { pos.x, pos.y, sprite_rect.w,sprite_rect.h };

	rect_sprite = sprite_rect;
	bar_value = value;
	bar_type = type;
	current_value = value->GetMaxValue();
}

void UIBar::DecreaseBar(uint value)
{
	if (bar_type == BarType::BAR_VERTICAL) {
		uint height = (rect_box.h / bar_value->GetMaxValue()) * value;
		rect_sprite.h -= height;
		rect_box.y += height;
		current_value -= value;
	}
	else if (bar_type == BarType::BAR_HORITZONTAL) {
		float width = (rect_box.w / bar_value->GetMaxValue()) * value;
		float aux = width - (int)width;
		decimal_decrease += aux;
		if (decimal_decrease > 1) {
			width += 1;
			decimal_decrease -= 1;
		}
		rect_sprite.w -= (int)width;
		current_value -= value;
	}
}

void UIBar::IncreaseBar(uint value)
{
	if (bar_type == BarType::BAR_VERTICAL) {
		uint height = (rect_box.h / bar_value->GetMaxValue()) * value;
		rect_sprite.h += height;
		rect_box.y -= height;
		current_value += value;
	}
	else if (bar_type == BarType::BAR_HORITZONTAL) {
		float width = (rect_box.w / bar_value->GetMaxValue()) * value;
		float aux = width - (int)width;
		decimal_increase += aux;
		if (decimal_increase > 1) {
			width += 1;
			decimal_increase -= 1;
		}
		rect_sprite.w += width;
		current_value += value;
	}
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
	LOG("Current Value: %i\n Bar Value: %f", current_value, bar_value->GetValue());
	if (bar_value->GetValue() > current_value) {
		IncreaseBar(bar_value->GetValue() - current_value);
	}
	else if (bar_value->GetValue() < current_value) {
		DecreaseBar(current_value - bar_value->GetValue());
	}

	
		

	return true;
}