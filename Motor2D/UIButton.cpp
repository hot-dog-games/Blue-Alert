#include "j1App.h"
#include "Audio.h"
#include "Render.h"
#include "GUI.h"
#include "UIButton.h"
#include "p2Log.h"

UIButton::~UIButton()
{
}

bool UIButton::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	if (clipping && parent)
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, &parent->GetScreenRect());
	else
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);
	return true;
}

void UIButton::OnMouseClick()
{
	rect_sprite = anim[2];
	App->audio->PlayFx(sound);
}

void UIButton::OnMouseHover()
{
	rect_sprite = anim[1];
}

void UIButton::OnMouseRelease()
{
	rect_sprite = anim[1];
}

void UIButton::OnMouseExit()
{
	rect_sprite = anim[0];
}

void UIButton::SetLocked(bool value)
{
	interactable = value;
	rect_sprite = anim[interactable ? 0 : 3];
}

bool UIButton::CleanUp()
{
	delete[] anim;
	parent = nullptr;
	return true;
}

UIButton::UIButton(iPoint position, SDL_Rect* sprite_rect, bool is_interactable)
{
	interactable = is_interactable;
	rect_box = { position.x, position.y, sprite_rect[0].w,sprite_rect[0].h };

	anim = new SDL_Rect[4];
	anim[0] = sprite_rect[0];
	anim[1] = sprite_rect[1];
	anim[2] = sprite_rect[2];
	anim[3] = sprite_rect[3];

	rect_sprite = anim[interactable ? 0 : 3];

	sound = App->audio->LoadFx("fx_button.wav");
}