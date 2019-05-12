#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIPopUp.h"
#include "UIButton.h"
#include "UILabel.h"
#include "p2Log.h"

UIPopUp::UIPopUp(SDL_Rect rect, std::string text, SDL_Color color)
{
	rect_box = rect;
	rect_sprite = { 3017, 1536, 212,193 };

	SDL_Rect button_rect[3];
	button_rect[0] = { 3231,1533,26,26 };
	button_rect[1] = { 3231,1533,26,26 };
	button_rect[2] = { 3231,1533,26,26 };
	popup_label = App->gui->CreateLabel({ 5,5 }, "fonts/red_alert.ttf", 20, text, color, rect.w, this);
	popup_button = App->gui->CreateButton({ rect.w - 10, rect.h - 10 }, button_rect, this);
}


UIPopUp::~UIPopUp()
{
}

bool UIPopUp::UIBlit()
{
	iPoint screen_pos = GetScreenPos();

	if (clipping && parent)
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, &parent->GetScreenRect());
	else
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);

	return true;
}

bool UIPopUp::Update(float dt)
{
	return true;
}
