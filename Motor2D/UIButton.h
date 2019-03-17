#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include"UIElement.h"

enum ButtonType;

class UIButton : public UIElement
{
public:
	UIButton(iPoint pos, ButtonType type, bool is_interactable = true);
	~UIButton();

	ButtonType type;
	SDL_Rect* anim;
	int sound;

	bool UIBlit();
	void OnMouseClick();
	void OnMouseHover();
	void OnMouseRelease();
	void OnMouseExit();
	void SetLocked(bool value);
	bool CleanUp();
};

#endif // !UI_BUTTON_H