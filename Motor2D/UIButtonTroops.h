#ifndef UI_BUTTON_TROOPS_H
#define UI_BUTTON_TROOPS_H

#include"UIButton.h"

enum ButtonLevel;

class UIButtonTroops : public UIButton
{
public:
	UIButtonTroops(iPoint pos, SDL_Rect* sprite_rect, bool is_interactable, ButtonLevel lvl);
	~UIButtonTroops();

	void OnMouseClick();
	void OnMouseHover();
	void OnMouseRelease();
	void OnMouseExit();

	void ChangeSprite(SDL_Rect * rect, ButtonLevel level);

	bool UIBlit();

private:
	SDL_Rect* mark;
	ButtonLevel level;

	SDL_Rect rect_sprite_mark;

};

#endif // !UI_BUTTON_TROOPS_H