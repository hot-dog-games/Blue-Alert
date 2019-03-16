#include "UIAnimatedImage.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Window.h"
#include "Render.h"

UIAnimatedImage::UIAnimatedImage(iPoint pos, SDL_Rect * rect, int total_sprites, int speed)
{
	rect_box.x = pos.x;
	rect_box.y = pos.y;
	for (int i = 0; i < total_sprites; i++)
	{
		animation.PushBack(rect[i]);
	}
	animation.speed = speed;
	animation.loop = true;
}

UIAnimatedImage::~UIAnimatedImage()
{
}

bool UIAnimatedImage::UIBlit()
{
	iPoint screen_pos = GetScreenPos();
	if (clipping && parent)
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &animation_frame, 0.0F, 0.0, INT_MAX, INT_MAX, &parent->GetScreenRect());
	else
		App->render->Blit(App->gui->GetAtlas(), screen_pos.x, screen_pos.y, &animation_frame, 0.0F, 0.0, INT_MAX, INT_MAX);
	return true;
}

bool UIAnimatedImage::Update(float dt)
{
	animation_frame = animation.GetCurrentFrame(dt);
	rect_box.h = animation_frame.h;
	rect_box.w = animation_frame.w;
	return true;
}
