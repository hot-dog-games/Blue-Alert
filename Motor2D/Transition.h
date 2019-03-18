#ifndef _TRANSITION_H_
#define _TRANSITION_H_

#include "Timer.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"
#include "Color.h"

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
		ZOOM,
		TRANSLATION
	};

private:
	TransitionType type;
	TransitionState state;

	float transition_time;
	Timer* current_time = nullptr;

	bool is_scene_change = false;
	int scene_to_change = 0;

	//---Specific transitions variables-------
	SDL_Rect screen;

	//Fade
	Color color;

	//Zoom
	float current_scale;
	float target_scale;
	float normal_scale = 1;

	//CameraTranslation
	iPoint origin;
	iPoint destination;

public:

	Transition();
	Transition(TransitionType type, float time, bool is_scene_change = false, int scene_to_change = 0);
	~Transition();

	void OnCreate();
	void PreUpdate();
	void Update();
	void PostUpdate();

	//------Getters & Setters------

	TransitionState GetState();
	void SetState(TransitionState state);
	TransitionType GetType();
	void SetColor(Color color);

	//-----Update process-------

	void Entering();
	void Action();
	void Exiting();

	//---Specific transitions methods-------

	void DrawFadeRect(float alpha_value);
	void SetScale(int scale);
	void SetOriginAndDestination(iPoint origin, iPoint destination);
};

#endif // _TRANSITION_H_