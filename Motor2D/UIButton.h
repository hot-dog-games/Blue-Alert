#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include"UIElement.h"


class UIButton : public UIElement
{
public:
	UIButton(iPoint pos, SDL_Rect* sprite_rect, bool is_interactable = true);
	~UIButton();

	SDL_Rect* anim;
	int sound;

	bool UIBlit();
	void OnMouseClick();
	void OnMouseHover();
	void OnMouseRelease();
	void OnMouseExit();

	void ChangeSprite(SDL_Rect* rect);
	void SetLocked(bool value);
	SDL_Rect* GetAnim() const;
	bool CleanUp();

//	std::string button_fx;
};

#endif // !UI_BUTTON_H