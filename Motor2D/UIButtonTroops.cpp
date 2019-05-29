#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIButtonTroops.h"

UIButtonTroops::~UIButtonTroops()
{
}

bool UIButtonTroops::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	if (clipping && parent)
	{
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, 1.0F, 1.0F, &parent->GetScreenRect());
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &mark, 0.0F, 0.0, INT_MAX, INT_MAX, 1.0F, 1.0F, &parent->GetScreenRect());
	}
	else {
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &mark, 0.0F, 0.0, INT_MAX, INT_MAX);
	}

	return true;
}


UIButtonTroops::UIButtonTroops(iPoint position, SDL_Rect* sprite_rect, bool is_interactable, ButtonLevel lvl) :UIButton(position, sprite_rect, is_interactable)
{
	level = lvl;

	switch (level) {
	case LVL_1:
		mark = { 2370,745,100,100 };
		break;
	case LVL_2:
		mark = { 2482,745,100,100 };
		break;
	case LVL_3:
		mark = { 2594,745,100,100 };
		break;
	case LVL_4:
		mark = { 2594,642,100,100 };
		break;
	}
}