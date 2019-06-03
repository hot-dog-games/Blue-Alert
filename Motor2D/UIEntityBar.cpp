#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIEntityBar.h"
#include "Stat.h"
#include "p2Log.h"
#include "p2Defs.h"
#include "Entity.h"

UIEntityBar::UIEntityBar(iPoint pos, SDL_Rect sprite_rect, Stat* value, BarType type, BarState state, Entity* entity): UIBar(pos,sprite_rect,value,type, state)
{
	this->entity = entity;
	entity_height = entity->current_frame.h;

	/*m creating a temporary surface, blitting the portion
		of the source surface to it, and converting it to a texture.*/

	bar_texture = SDL_CreateTexture(App->render->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sprite_rect.w, sprite_rect.h);
	SDL_SetRenderTarget(App->render->renderer, bar_texture);
	SDL_RenderCopy(App->render->renderer, App->gui->GetAtlas(), &sprite_rect, nullptr);
	SDL_SetRenderTarget(App->render->renderer, nullptr);

	ChangeColor();
}

bool UIEntityBar::Update(float dt)
{
	if (bar_value->GetValue() > current_value) {
		IncreaseBar(bar_value->GetValue() - current_value);
		ChangeColor();
	}
	else if (bar_value->GetValue() < current_value) {
		DecreaseBar(current_value - bar_value->GetValue());
		ChangeColor();
	}

	iPoint entity_pos = App->render->WorldToScreen((int)entity->position.x, (int)entity->position.y);
	rect_box.x = entity_pos.x - rect_box.w * 0.5;
	rect_box.y = entity_pos.y - entity_height - bar_margin;

	return true;
}

bool UIEntityBar::UIBlit()
{
	SDL_Rect rect = { 0, 0, rect_sprite.w, rect_sprite.h };
	iPoint screen_pos = GetScreenPos();
	if (clipping && parent)
		App->render->Blit(bar_texture, screen_pos.x, screen_pos.y, &rect, 0.0F, 0.0, INT_MAX, INT_MAX, scale_X, scale_Y, &parent->GetScreenRect());
	else
		App->render->Blit(bar_texture, screen_pos.x, screen_pos.y, &rect, 0.0F, 0.0, INT_MAX, INT_MAX);
	return true;
}

void UIEntityBar::ChangeColor()
{
	float max_value = bar_value->GetMaxValue();
	float r, g, b;
	b = 0;

	if (current_value >= max_value*0.5f)
	{
		r = LINEAR_INTERPOLATION(current_value, max_value * 0.5f, 255.0f, max_value, 0.0f);
		g = 255.0f;
	}
	else
	{
		r = 255.0f;
		g = LINEAR_INTERPOLATION(current_value, 0, 0.0f, max_value * 0.5f, 255.0f);
	}
	SDL_SetTextureColorMod(bar_texture, (int)r, (int)g, (int)b);
}

bool UIEntityBar::CleanUp()
{
	SDL_DestroyTexture(bar_texture);
	bar_texture = nullptr;
}
