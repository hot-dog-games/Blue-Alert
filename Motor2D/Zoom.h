#pragma once
#include "Transition.h"
class Zoom :
	public Transition
{

private:
	float current_scale = 1;
	float target_scale;
	float normal_scale = 1;

public:
	Zoom(float transition_time, float target_scale);
	~Zoom();

	void Entering();
	void Action();
	void Exiting();

	void SetTargetScale(int target_scale);
};

