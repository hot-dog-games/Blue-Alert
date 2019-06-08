#include "j1App.h"
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
	slider_button = App->gui->CreateButton({ (int)initial_value, -6 }, slider, this);

	percentage = initial_value / rect_box.w;

	minimum = pos.x - this->GetLocalPos().x;

	maximum = pos.x + rect.w - slider_button->GetLocalRect().w - this->GetLocalPos().x;
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

	int pos_x = mouse.x - this->GetLocalPos().x - slider_button->GetLocalRect().w / 2;

	if (!parent)
		slider_button->SetLocalPos(pos_x, slider_button->GetLocalPos().y);
	else
		slider_button->SetLocalPos(pos_x - (int)parent->GetLocalPos().x, slider_button->GetLocalPos().y);
	
	if (slider_button->GetLocalPos().x < minimum)
	{
		slider_button->SetLocalPos(minimum, slider_button->GetLocalPos().y);
	}
	if (slider_button->GetLocalPos().x > maximum)
	{
		slider_button->SetLocalPos(maximum, slider_button->GetLocalPos().y);
	}

	percentage = (float)(pos_x - (int)parent->GetLocalPos().x) / 194;
	current_value = percentage * max_value;

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
