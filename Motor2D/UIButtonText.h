#ifndef UI_BUTTON_TEXT_H
#define UI_BUTTON_TEXT_H

#include"UIButton.h"

class UILabel;

class UIButtonText : public UIButton
{
public:
	UIButtonText(iPoint pos, iPoint offset, SDL_Rect* sprite_rect, std::string text, SDL_Color color = {0,0,0,0}, int size = 20, bool is_interactable = true);
	~UIButtonText();

	void OnMouseClick();
	void OnMouseHover();
	void OnMouseRelease();
	void OnMouseExit();

	bool CleanUp();

private:
	UILabel* button_label = nullptr;
};

#endif // !UI_BUTTON_TEXT_H