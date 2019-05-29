#ifndef UI_BUTTON_TROOPS_H
#define UI_BUTTON_TROOPS_H

#include"UIButton.h"

enum ButtonLevel;

class UIButtonTroops : public UIButton
{
public:
	UIButtonTroops(iPoint pos, SDL_Rect* sprite_rect, bool is_interactable, ButtonLevel lvl);
	~UIButtonTroops();

	bool UIBlit();


private:
	SDL_Rect mark;
	ButtonLevel level;

};

#endif // !UI_BUTTON_TROOPS_H