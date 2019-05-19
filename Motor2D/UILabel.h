#ifndef UI_LABEL_H
#define UI_LABEL_H

#include "UIElement.h"
#include <string>

struct _TTF_Font;

class UILabel : public UIElement
{
public:
	_TTF_Font* font;
	std::string text = "";
	SDL_Color color;
	SDL_Color original_color;
	UILabel(iPoint pos, _TTF_Font* font, std::string text, SDL_Color color, int max_width, bool is_interactable);
	~UILabel();
	bool UIBlit();
	void SetText(std::string text);
	void SetColor(SDL_Color color);
	bool CleanUp();

	void OnMouseClick();
	void OnMouseHover();
	void OnMouseRelease();
	void OnMouseExit();

};

#endif // !UI_LABEL_H