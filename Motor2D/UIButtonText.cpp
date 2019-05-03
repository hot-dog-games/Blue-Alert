#include "j1App.h"
#include "GUI.h"
#include "Fonts.h"
#include "UILabel.h"
#include "p2Log.h"
#include "UIButtonText.h"


UIButtonText::~UIButtonText()
{
}

void UIButtonText::OnMouseClick()
{
	rect_sprite = anim[2];
	button_label->OnMouseClick();
}

void UIButtonText::OnMouseHover()
{
	rect_sprite = anim[1];
	button_label->OnMouseHover();
}

void UIButtonText::OnMouseRelease()
{
	rect_sprite = anim[1];
	button_label->OnMouseRelease();
}

void UIButtonText::OnMouseExit()
{
	rect_sprite = anim[0];
	button_label->OnMouseExit();
}

bool UIButtonText::CleanUp()
{
	delete[] anim;
	parent = nullptr;
	return true;
}

UIButtonText::UIButtonText(iPoint position, iPoint offset, SDL_Rect* sprite_rect, std::string text, SDL_Color color, int size, bool is_interactable): UIButton(position,sprite_rect,is_interactable)
{
	button_label = App->gui->CreateLabel({ offset.x, (int)(rect_sprite.h * 0.5 - size*0.5)}, "fonts/button_text.ttf", size, text, color, 1000, this, false);
}