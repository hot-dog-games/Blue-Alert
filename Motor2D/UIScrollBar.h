#ifndef UI_SCROLLBAR_H
#define UI_SCROLLBAR_H

#include "UIElement.h"
#include "UIImage.h"

enum ScrollType;

class UIScrollBar : public UIElement
{
public:
	UIScrollBar(iPoint pos, SDL_Rect rect, float initial_value);
	~UIScrollBar();
	void Update();
	void DragSlider();
	void CalculateValue();
	bool UIBlit();


public:
	float value = min;

private:
	SDL_Rect slider[3];
	uint min = 0;
	uint max = 0;
	UIButton* slider_button;


};

#endif // !UI_SCROLLBAR_H