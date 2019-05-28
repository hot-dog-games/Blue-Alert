#include "j1App.h"
#include "Render.h"
#include "GUI.h"
#include "UIImage.h"
UIImage::UIImage(iPoint pos, SDL_Rect rect, bool image)
{
	rect_box = { pos.x,pos.y,rect.w,rect.h };
	this->rect_sprite = rect;
	this->image = image;
}

UIImage::UIImage()
{
}

UIImage::~UIImage()
{
}

bool UIImage::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	if (image)
	{
		if (clipping && parent)
			App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX, scale_X, scale_Y, &parent->GetScreenRect());
		else
			App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &rect_sprite, 0.0F, 0.0, INT_MAX, INT_MAX);
	}
	return true;
}

void UIImage::SetImage(SDL_Rect rect)
{
	rect_sprite = rect;
}
