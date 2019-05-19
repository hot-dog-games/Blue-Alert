 #include "j1App.h"
#include "GUI.h"
#include "UIElement.h"
#include "p2Log.h"


UIElement::UIElement(UIElement* parent)
{
	this->parent = parent;
}

UIElement::~UIElement()
{
}

bool UIElement::UIBlit()
{
	return true;
}

bool UIElement::UICleanUp()
{
	parent = nullptr;
	return true;
}

bool UIElement::IsInside(int x, int y)
{
	SDL_Rect screen_rect = GetScreenRect();
	return((x < screen_rect.x + screen_rect.w) && (screen_rect.x < x) && (y < screen_rect.y + screen_rect.h) && (screen_rect.y < y));
}

SDL_Rect UIElement::GetScreenRect()
{
	if (parent)
		return { (int)(parent->GetScreenPos().x + (rect_box.x * parent->scale_X)), (int)(parent->GetScreenPos().y + (rect_box.y * parent->scale_Y)), (int)(rect_box.w * scale_X), (int)(rect_box.h * scale_Y) };
	else
		return  { rect_box.x, rect_box.y, (int)(rect_box.w * scale_X), (int)(rect_box.h * scale_Y) };
}

void UIElement::SetScreenPos(int x, int y)
{
	if (parent)
	{
		rect_box.x = (x - parent->GetScreenPos().x) / parent->scale_X;
		rect_box.y = (y - parent->GetScreenPos().y) / parent->scale_Y;
	}
	else
	{
		rect_box.x = rect_box.x;
		rect_box.y = rect_box.y;
	}
}

SDL_Rect UIElement::GetLocalRect()
{
	return { rect_box.x, rect_box.y, (int)(rect_box.w*scale_X), (int)(rect_box.h*scale_Y) };
}

iPoint UIElement::GetScreenPos()
{
	if (parent)
		return { (int)(parent->GetScreenPos().x + (rect_box.x * parent->scale_X)), (int)(parent->GetScreenPos().y + (rect_box.y * parent->scale_Y)) };
	else
		return { rect_box.x, rect_box.y };
}

iPoint UIElement::GetLocalPos()
{
	return { rect_box.x, rect_box.y };
}

void UIElement::SetLocalPos(int x, int y)
{
	rect_box.x = x;
	rect_box.y = y;
}

void UIElement::GetScale(float& scaleX, float &scaleY)
{
	scaleX = scale_X;
	scaleY = scale_Y;
}

void UIElement::SetScale(float scaleX, float scaleY)
{
	scale_X = scaleX;
	scale_Y = scaleY;
}

void UIElement::SetLocalRect(SDL_Rect rect)
{
	rect_box = rect;
}

void UIElement::DadEnabled()
{
	if (parent != nullptr)
	{
		if (!parent->enabled)
			this->enabled = false;
		else
			this->enabled = true;
	}
}

void UIElement::SetEnabled(bool enabled)
{
	this->enabled = enabled;
}


bool UIElement::IsEnabled() const
{
	return enabled;
}

void UIElement::UpdateRect()
{
	rect_sprite.x = (int)(rect_sprite.x * scale_X);
	rect_sprite.y = (int)(rect_sprite.y*scale_Y);
}

