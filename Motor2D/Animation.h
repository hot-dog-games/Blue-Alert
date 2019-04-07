#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Animation
{
public:
	bool loop = true;
	bool hold = false;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	const int GetFrameAmount() const
	{
		return last_frame;
	}

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;
public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	bool isDone()
	{
		if (current_frame == last_frame - 1) return true;
		else return false;
	}

	float GetCurrentFrameIndex()
	{
		return current_frame;
	}

	void setCurrentFrameIndex(float i)
	{
		current_frame = i;
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		if (!hold)
		{
			current_frame += speed * dt;
			if (current_frame >= last_frame)
			{
				current_frame = (loop) ? 0.0f : last_frame - 1;
				loops++;
			}
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset() {
		current_frame = 0;
	}
};

#endif