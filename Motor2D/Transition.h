#ifndef _TRANSITION_H_
#define _TRANSITION_H_

#include "Timer.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

class Transition
{
private:
	enum class TransitionState {
		NONE,
		ENTERING,
		ACTION,
		EXITING
	};

public:
	enum class TransitionType {
		FADE,
		ZOOM
	};

private:
	TransitionType m_type;
	TransitionState m_state;

	float transition_time;
	Timer* current_time = nullptr;

	bool is_scene_change = false;
	int scene_to_change = 0;

	//---Specific transitions variables-------
	SDL_Rect screen;

public:

	Transition();
	Transition(TransitionType type, float time, bool is_scene_change = false, int scene_to_change = 0);
	~Transition();

	void OnCreate();
	void PreUpdate();
	void Update();
	void PostUpdate();

	TransitionState GetState();
	void SetState(TransitionState state);
	TransitionType GetType();

	//---------------------------

	void Entering();
	void Action();
	void Exiting();

	//---Specific transitions methods-------

	void DrawFadeRect();
};

#endif // _TRANSITION_H_