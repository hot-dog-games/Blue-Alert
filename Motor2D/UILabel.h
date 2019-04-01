#ifndef UI_LABEL_H
#define UI_LABEL_H


#include "UIElement.h"
#include <string>

class _TTF_Font;

class UILabel : public UIElement
{
public:
	_TTF_Font* font;
	std::string text;
	SDL_Color color;
	UILabel(iPoint pos, _TTF_Font* font, std::string text, SDL_Color color, int max_width);
	~UILabel();
	bool UIBlit();
	void SetText(std::string text);
	void SetColor(SDL_Color color);
	bool CleanUp();
};

#endif // !UI_LABEL_H