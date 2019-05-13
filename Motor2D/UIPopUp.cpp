#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIPopUp.h"
#include "UIButton.h"
#include "UILabel.h"
#include "p2Log.h"

UIPopUp::UIPopUp(SDL_Rect rect, std::string text, int text_size, SDL_Color color)
{
	rect_box = rect;

	//Check if rectangle shape or square
	if (rect_box.w > (int)(rect_box.h * 1.5))
		rect_sprite = { 3282, 1538, 374,192 };
	else
		rect_sprite = { 3017, 1536, 212,193 };

	//Creates PopUp

	SDL_Rect button_rect[3];
	button_rect[0] = { 3231,1533,26,26 };
	button_rect[1] = { 3231,1559,26,26 };
	button_rect[2] = { 3231,1585,26,26 };

	SetScale((float)rect_box.w / rect_sprite.w, (float)rect_box.h / rect_sprite.h);

	popup_label = App->gui->CreateLabel({ margin_label.x, margin_label.y}, "fonts/red_alert.ttf", 20, text, color, rect_box.w - button_rect->w - margin_button.x - margin_label.x, this);
	popup_button = App->gui->CreateButton({ rect_box.w - button_rect->w - margin_button.x,(int)((rect_box.h - button_rect->h - margin_button.y)*scale_Y) }, button_rect, this);

}


UIPopUp::~UIPopUp()
{
}

bool UIPopUp::UIBlit()
{
	iPoint screen_pos = GetScreenPos();


	if (clipping && parent)
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, scale_X, scale_Y, &parent->GetScreenRect());
	else
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, scale_X, scale_Y);


	return true;
}

bool UIPopUp::Update(float dt)
{
	if (popup_button->clicked)
	{
		App->gui->DeleteElement(this);
	}

	return true;
}
