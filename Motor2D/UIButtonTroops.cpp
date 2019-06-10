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
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite_mark, 0.0F, 0.0, INT_MAX, INT_MAX, 1.0F, 1.0F, &parent->GetScreenRect());
	}
	else {
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite_mark, 0.0F, 0.0, INT_MAX, INT_MAX);
	}

	return true;
}

void UIButtonTroops::OnMouseClick()
{
	rect_sprite = anim[2];
	rect_sprite_mark = mark[2];
}

void UIButtonTroops::OnMouseHover()
{
	rect_sprite = anim[1];
	rect_sprite_mark = mark[1];
}

void UIButtonTroops::OnMouseRelease()
{
	rect_sprite = anim[1];
	rect_sprite_mark = mark[1];
}

void UIButtonTroops::OnMouseExit()
{
	rect_sprite = anim[0];
	rect_sprite_mark = mark[0];
}

void UIButtonTroops::ChangeSprite(SDL_Rect* rect, ButtonLevel level)
{
	anim[0] = rect[0];
	anim[1] = rect[1];
	anim[2] = rect[2];
	anim[3] = rect[3];
	rect_sprite = anim[0];

	switch (level) {
	case LVL_1:
		mark[0] = { 2370,745,100,100 };
		mark[1] = { 2370,333,100,100 };
		mark[2] = { 2370,539,100,100 };
		mark[3] = { 2482,436,100,100 };
		break;

	case LVL_2:
		mark[0] = { 2482,745,100,100 };
		mark[1] = { 2482,333,100,100 };
		mark[2] = { 2482,539,100,100 };
		mark[3] = { 2482,436,100,100 };
		break;

	case LVL_3:
		mark[0] = { 2594,745,100,100 };
		mark[1] = { 2594,333,100,100 };
		mark[2] = { 2594,539,100,100 };
		mark[3] = { 2482,436,100,100 };
		break;

	case LVL_4:
		mark[0] = { 2594,642,100,100 };
		mark[1] = { 2594,230,100,100 };
		mark[2] = { 2594,539,100,100 };
		mark[3] = { 2482,436,100,100 };
		break;

	case LVL_5:
		mark[0] = { 2370,642,100,100 };
		mark[1] = { 2370,230,100,100 };
		mark[2] = { 2370,539,100,100 };
		mark[3] = { 2482,436,100,100 };
		break;

	default:
		mark[0] = { 2482,436,100,100 };
		mark[1] = { 2482,436,100,100 };
		mark[2] = { 2482,436,100,100 };
		mark[3] = { 2482,436,100,100 };
		break;
	}

	rect_sprite_mark = mark[interactable ? 0 : 3];
}

UIButtonTroops::UIButtonTroops(iPoint position, SDL_Rect* sprite_rect, bool is_interactable, ButtonLevel lvl, ButtonTroopType type) :UIButton(position, sprite_rect, is_interactable)
{
	level = lvl;

	mark = new SDL_Rect[4];
	if (type == COLLECTION) {
		switch (level) {
		case LVL_1:
			mark[0] = { 2370,745,100,100 };
			mark[1] = { 2370,333,100,100 };
			mark[2] = { 2370,539,100,100 };
			mark[3] = { 2482,436,100,100 };
			break;

		case LVL_2:
			mark[0] = { 2482,745,100,100 };
			mark[1] = { 2482,333,100,100 };
			mark[2] = { 2482,539,100,100 };
			mark[3] = { 2482,436,100,100 };
			break;

		case LVL_3:
			mark[0] = { 2594,745,100,100 };
			mark[1] = { 2594,333,100,100 };
			mark[2] = { 2594,539,100,100 };
			mark[3] = { 2482,436,100,100 };
			break;

		case LVL_4:
			mark[0] = { 2594,642,100,100 };
			mark[1] = { 2594,230,100,100 };
			mark[2] = { 2594,539,100,100 };
			mark[3] = { 2482,436,100,100 };
			break;

		case LVL_5:
			mark[0] = { 2370,642,100,100 };
			mark[1] = { 2370,230,100,100 };
			mark[2] = { 2370,436,100,100 };
			mark[3] = { 2482,436,100,100 };
			break;

		default:
			mark[0] = { 2482,436,100,100 };
			mark[1] = { 2482,436,100,100 };
			mark[2] = { 2482,436,100,100 };
			mark[3] = { 2482,436,100,100 };
			break;
		}
	}
	else if (type == IN_GAME)
	{
		switch (level) {
		case LVL_1:
			mark[0] = { 1814,362,101,80 };
			mark[1] = { 2033,362,101,80 };
			mark[2] = { 2252,362,101,80 };
			mark[3] = { 2252,529,101,80 };
			break;

		case LVL_2:
			mark[0] = { 1706,445,101,80 };
			mark[1] = { 1925,445,101,80 };
			mark[2] = { 2144,445,101,80 };
			mark[3] = { 2252,529,101,80 };
			break;

		case LVL_3:
			mark[0] = { 1706,362,101,80 };
			mark[1] = { 1925,362,101,80 };
			mark[2] = { 2144,362,101,80 };
			mark[3] = { 2252,529,101,80 };
			break;

		case LVL_4:
			mark[0] = { 1814,445,101,80 };
			mark[1] = { 2033,445,101,80 };
			mark[2] = { 2252,445,101,80 };
			mark[3] = { 2252,529,101,80 };
			break;

		case LVL_5:
			mark[0] = { 1706,529,101,80 };
			mark[1] = { 1925,529,101,80 };
			mark[2] = { 2144,529,101,80 };
			mark[3] = { 2252,529,101,80 };
			break;

		default:
			mark[0] = { 2252,529,101,80 };
			mark[1] = { 2252,529,101,80 };
			mark[2] = { 2252,529,101,80 };
			mark[3] = { 2252,529,101,80 };
			break;
		}
	}

	rect_sprite_mark = mark[interactable ? 0 : 3];
}