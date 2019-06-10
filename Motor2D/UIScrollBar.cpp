#include "j1App.h"
#include "p2Defs.h"
#include "Window.h"
#include "GUI.h"
#include "UIElement.h"
#include "UIScrollBar.h"
#include "UIButton.h"
#include "Render.h"
#include "Input.h"
#include "p2Log.h"

UIScrollBar::UIScrollBar(iPoint pos, SDL_Rect rect, SliderType type, int initial_value, int max_value) {
	rect_box = { pos.x,pos.y,rect.w,rect.h };
	this->rect_sprite = rect;
	slidertype = type;
	slider[0] = { 771,1309,24,50 };
	slider[1] = { 804,1309,24,50 };
	slider[2] = { 839,1309,24,50 };

	slider_button = App->gui->CreateButton({ (int)initial_value, 0 }, slider, this);
	SDL_Rect screen_rect = slider_button->GetScreenRect();
	SDL_Rect parent_rect = GetScreenRect();
	slider_button->SetScreenPos(LINEAR_INTERPOLATION(initial_value, 0, parent_rect.x, max_value, parent_rect.x + parent_rect.w) / App->win->GetScale() , (parent_rect.y - 4) / App->win->GetScale());

	percentage = initial_value / rect_box.w;

	minimum = pos.x - this->GetLocalPos().x;

	maximum = pos.x + rect.w - slider_button->GetScreenRect().w - this->GetLocalPos().x;
	current_value = initial_value;
	this->max_value = max_value;

	interactable = true;

}
UIScrollBar::~UIScrollBar()
{

}

bool UIScrollBar::Update(float dt)
{
	if (slider_button->clicked || this->clicked)
	{
		DragSlider();
	}

	return true;
}

void UIScrollBar::DragSlider()
{
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);
	SDL_Rect screen_rect = slider_button->GetScreenRect();
	SDL_Rect parent_rect = GetScreenRect();
	mouse.x -= App->render->scaled_viewport.x / App->win->GetScale();
	mouse.y -= App->render->scaled_viewport.y / App->win->GetScale();

	slider_button->SetScreenPos(mouse.x - (screen_rect.w / 2), (parent_rect.y - 4) / App->win->GetScale());

	if (parent_limit && parent)
	{
		screen_rect = slider_button->GetScreenRect();
		if (screen_rect.x < parent_rect.x)
			screen_rect.x = parent_rect.x;
		else if (screen_rect.x + screen_rect.w > parent_rect.x + parent_rect.w)
			screen_rect.x = (parent_rect.x + parent_rect.w) - screen_rect.w;
		if (screen_rect.y <= parent_rect.y)
			screen_rect.y = parent_rect.y;
		else if (screen_rect.y + screen_rect.h >= parent_rect.y + parent_rect.h)
			screen_rect.y = (parent_rect.y + parent_rect.h) - screen_rect.h;

		slider_button->SetScreenPos(screen_rect.x / App->win->GetScale(), (parent_rect.y - 4) / App->win->GetScale());
	}
	screen_rect = slider_button->GetScreenRect();
	current_value = LINEAR_INTERPOLATION(screen_rect.x, parent_rect.x, 0, parent_rect.x + parent_rect.w, max_value);

	App->gui->SliderAction(slidertype, this);

}


bool UIScrollBar::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
		if (clipping && parent)
			App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, scale_X, scale_Y, &parent->GetScreenRect());
		else
			App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);
	return true;
}
