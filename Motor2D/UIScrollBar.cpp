#include"j1App.h"
#include "GUI.h"
#include "UIScrollBar.h"
#include "UIButton.h"
#include"Render.h"
#include"Input.h"
#include"p2Log.h"

UIScrollBar::UIScrollBar(iPoint pos, SDL_Rect rect, float initial_value){
	rect_box = { pos.x,pos.y,rect.w,rect.h };
	this->rect_sprite = rect;

	slider[0] = { 3592, 2264,24,51 };
	slider[1] = { 3625, 2264,25,51 };
	slider[2] = { 3660, 2264,25,51 };
	slider_button = App->gui->CreateButton({ (int)initial_value, -6 }, slider,this);
	
	
	min = pos.x;
	max = pos.x + rect.w - slider_button->GetLocalRect().w;
	value = initial_value;
	
}
UIScrollBar::~UIScrollBar()
{

}

void UIScrollBar::Update()
{
	
	if (slider_button->clicked)
	{
		LOG("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
		DragSlider();
	}
	
}

void UIScrollBar::DragSlider()
{
		iPoint mouse;
		App->input->GetMousePosition(mouse.x, mouse.y);
		slider_button->SetLocalPos(mouse.x, slider_button->GetLocalPos().y);


		/*if (position.x < min)
		{
			position.x = min;
		}
		if (position.x > max)
		{
			position.x = max;
		}

		CalculateValue();
		App->gui->SliderAction(this);*/
	
}

void UIScrollBar::CalculateValue()
{
	//value = position.x - min;
}

bool UIScrollBar::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	
		if (clipping && parent)
			App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, &parent->GetScreenRect());
		else
			App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);
	
	return true;
}