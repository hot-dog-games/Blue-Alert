#ifndef UI_SCROLLBAR_H
#define UI_SCROLLBAR_H

#include "UIElement.h"
#include "UIImage.h"

enum SliderType;

class UIScrollBar : public UIElement
{
public:
	UIScrollBar(iPoint pos, SDL_Rect rect, SliderType type, int initial_value, int max_value);
	~UIScrollBar();
	bool Update(float dt);
	void DragSlider();
	bool UIBlit();


public:
	int* value;
	float current_value = 0;
private:
	SDL_Rect slider[3];
	SliderType slidertype;
	int minimum = 0;
	int maximum = 0;

	UIButton* slider_button;


	double percentage = 0;

	int max_value = 0;


};

#endif // !UI_SCROLLBAR_H