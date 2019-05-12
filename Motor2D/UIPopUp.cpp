#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIPopUp.h"

UIPopUp::UIPopUp(SDL_Rect rect, std::string text, SDL_Color color)
{
	rect_box = rect;
	rect_sprite = { 3017, 1536, 212,193 };

	SDL_Rect button_rect[3];
	/*button_rect[0] = {}*/
	popup_label = App->gui->CreateLabel({ 5,5 }, "fonts/button_text.ttf", 20, text, color, rect.w);
	popup_button = App->gui->CreateButton({rect.w - 10, rect.h - 10}, )
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
