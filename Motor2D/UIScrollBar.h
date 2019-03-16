#ifndef UI_SCROLLBAR_H
#define UI_SCROLLBAR_H

#include "UIElement.h"
#include "UIImage.h"

class UIScrollBar : public UIElement
{
public:
	UIScrollBar(iPoint pos, ScrollType type);
	~UIScrollBar() {};

	ScrollType type;
	UIImage* thumb = nullptr;
	float norm_value = 0.0F;
	float min, max;

	bool UIBlit();
	/*void OnMouseClick();
	void OnMouseHover();
	void OnMouseRelease();
	void OnMouseExit();
	void MoveOtherElement();*/
	void SetValue(float new_value);
	float GetValue();
	void SetMinMax(float min, float max);
	bool CleanUp();
};

#endif // !UI_SCROLLBAR_H