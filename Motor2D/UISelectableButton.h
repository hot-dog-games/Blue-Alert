#ifndef UI_BUTTON_SELECTABLE_H
#define UI_BUTTON_SELECTABLE_H

#include"UIButton.h"


class UISelectableButton : public UIButton
{
public:
	UISelectableButton(iPoint pos, SDL_Rect* sprite_rect, bool is_interactable = true);
	~UISelectableButton();

	void OnMouseClick();
	void OnMouseHover();
	void OnMouseRelease();
	void OnMouseExit();

	void ChangeState(bool state);
	bool IsSelected();

	bool selected = false;
};

#endif // !UI_BUTTON_H