#ifndef UI_SCROLLBAR_H
#define UI_SCROLLBAR_H

#include "UIElement.h"
#include "UIImage.h"

enum ScrollType;

class UIScrollBar : public UIElement
{
public:
	UIScrollBar(iPoint pos, SDL_Rect rect, int initial_value, int max_value);
	~UIScrollBar();
	bool Update(float dt);
	void DragSlider();
	void CalculateValue();
	bool UIBlit();


public:
	int* value;

private:
	SDL_Rect slider[3];

	int minimum = 0;
	int maximum = 0;

	UIButton* slider_button;

	float current_value = 0;
	double percentage = 0;

	int max_value = 0;


};

#endif // !UI_SCROLLBAR_H