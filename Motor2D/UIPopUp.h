#ifndef UI_POPUP_H
#define UI_POPUP_H

#include "UIElement.h"
#include <string>

class UIPopUp : public UIElement
{
public:
	UIPopUp(SDL_Rect rect, std::string text, SDL_Color color);
	~UIPopUp();
	bool UIBlit();

	bool Update(float dt);

private:
	UILabel* popup_label = nullptr;
	UIButton* popup_button = nullptr;
};

#endif // UI_POPUP_H