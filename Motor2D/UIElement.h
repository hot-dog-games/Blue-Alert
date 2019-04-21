#ifndef UI_ELEMENT_H
#define UI_ELEMENT_H

#include"p2Point.h"
#include"SDL/include/SDL_rect.h"


class UIElement
{
protected:
	SDL_Rect rect_box;
	SDL_Rect rect_sprite;
	float scale_X = 1.0F;
	float scale_Y = 1.0F;

public:
	bool hovered = false;
	bool interactable = false;
	bool dragable = false;
	bool clicked = false;
	bool enabled = true;
	bool selected = false;
	bool selectable = false;
	//TODO
	bool clipping = true; //Zona que se pinta es el rectangulo del padre.
	bool parent_limit = true;

	float starting_scale_X = 1.0F;
	float starting_scale_Y = 1.0F;
	float final_scale_X = 1.0F;
	float final_scale_Y = 1.0F;


	UIElement(UIElement* parent = nullptr);
	~UIElement();
	virtual bool UIBlit();
	virtual bool UICleanUp();
	virtual bool IsInside(int x, int y);
	virtual bool Update(float dt) { return true; };
	virtual bool CleanUp() { return true; };

	SDL_Rect GetScreenRect();
	SDL_Rect GetLocalRect();
	iPoint GetScreenPos();
	iPoint GetLocalPos();
	void SetLocalPos(int x, int y);
	void GetScale(float& scaleX, float &scaleY);
	void SetScale(float scaleX, float scaleY);
	void SetScreenPos(int x, int y);
	void SetLocalRect(SDL_Rect rect);
	void DadEnabled();
	void SetEnabled(bool enabled);

	virtual void OnMouseClick() {};
	virtual void OnMouseHover() {};
	virtual void OnMouseRelease() {};
	virtual void OnMouseExit() {};

	UIElement* parent = nullptr;
};

#endif // !UI_ELEMENT_H