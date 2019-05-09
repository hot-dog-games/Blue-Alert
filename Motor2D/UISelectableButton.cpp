#include "j1App.h"
#include "Audio.h"
#include "Render.h"
#include "GUI.h"
#include "UISelectableButton.h"
#include "p2Log.h"

UISelectableButton::~UISelectableButton()
{
}

void UISelectableButton::OnMouseClick()
{
	selected = !selected;
}

void UISelectableButton::OnMouseHover()
{

	rect_sprite = anim[1];

}

void UISelectableButton::OnMouseRelease()
{
	if (selected) {
		rect_sprite = anim[2];
	}
	else {
		rect_sprite = anim[0];
	}
}

void UISelectableButton::OnMouseExit()
{
	if (selected) {
		rect_sprite = anim[2];
	}
	else {
		rect_sprite = anim[0];
	}
}

void UISelectableButton::ChangeState(bool state)
{
	selected = state;

	if (selected) {
		rect_sprite = anim[2];
	}
	else {
		rect_sprite = anim[0];
	}
}

bool UISelectableButton::IsSelected()
{
	return selected;
}

UISelectableButton::UISelectableButton(iPoint position, SDL_Rect* sprite_rect, bool is_interactable) :UIButton(position, sprite_rect, is_interactable)
{
	anim = new SDL_Rect[4];
	anim[0] = sprite_rect[2];
	anim[1] = sprite_rect[1];
	anim[2] = sprite_rect[0];
	anim[3] = sprite_rect[3];

	rect_sprite = anim[interactable ? 0 : 3];
}