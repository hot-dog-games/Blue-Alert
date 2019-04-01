#ifndef UI_ANIMATEDIMAGE_H
#define UI_ANIMATEDIMAGE_H


#include "UIImage.h"
#include "Animation.h"

class UIAnimatedImage : public UIImage
{
public:
	UIAnimatedImage(iPoint pos, SDL_Rect* rect, int total_sprites, int speed);
	~UIAnimatedImage();
	bool UIBlit();
	bool Update(float dt);
	Animation animation;
	SDL_Rect animation_frame;
};
#endif // !UI_ANIMATEDIMAGE_H