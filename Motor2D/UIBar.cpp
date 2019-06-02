#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIBar.h"
#include "Stat.h"
#include "p2Log.h"

UIBar::UIBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, BarType type, BarState state)
{
	rect_box = { pos.x, pos.y, sprite_rect.w,sprite_rect.h };

	rect_sprite = sprite_rect;
	bar_value = value;
	bar_type = type;
	bar_state = state;
	if(state == BAR_DYNAMIC)
		current_value = value->GetMaxValue();
	else
		current_value = rect_sprite.w;
}

void UIBar::DecreaseBar(float value)
{
	if (bar_type == BarType::BAR_VERTICAL) {
		float height = (rect_box.h / bar_value->GetMaxValue()) * value;
		float aux = height - (int)height;
		decimal_decrease += aux;
		if (decimal_decrease > 1) {
			height += 1;
			decimal_decrease -= 1;
		}
		rect_sprite.h -= (int)height;
		rect_box.y += (int)height;
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

void UIBar::IncreaseBar(float value)
{
	if (bar_type == BarType::BAR_VERTICAL) {
		float height = (rect_box.h / bar_value->GetMaxValue()) * value;
		float aux = height - (int)height;
		decimal_decrease += aux;
		if (decimal_decrease > 1) {
			height += 1;
			decimal_decrease -= 1;
		}
		rect_sprite.h += (int)height;
		rect_box.y -= (int)height;
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
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, scale_X, scale_Y, &parent->GetScreenRect());
	else
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);
	return true;
}

bool UIBar::Update(float dt)
{
	if (bar_state == BAR_DYNAMIC) {
		if (bar_value->GetValue() > current_value) {
			IncreaseBar(bar_value->GetValue() - current_value);
		}
		else if (bar_value->GetValue() < current_value) {
			DecreaseBar(current_value - bar_value->GetValue());
		}
	}

	return true;
}

void UIBar::ChangeStat(Stat * stat)
{
	bar_value = stat;
	rect_sprite.w = bar_value->GetValue() * (current_value * 0.2);
}

int UIBar::GetValue() const
{
	return current_value;
}
