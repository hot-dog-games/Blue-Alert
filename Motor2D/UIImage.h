#ifndef UI_IMAGE_H
#define UI_IMAGE_H

#include "UIElement.h"

class UIImage : public UIElement
{
public:
	UIImage(iPoint pos, SDL_Rect rect, bool image = true);
	UIImage();
	~UIImage();
	bool UIBlit();
	bool image = true;

	void SetImage(SDL_Rect rect);
	void SetHeight(int h);
};

#endif // !1