#pragma once
#include "Transition.h"

class CameraTranslation :
	public Transition
{
private:
	iPoint origin;
	iPoint destination;

	float distance;
	iPoint direction;
public:
	CameraTranslation(float transition_time, iPoint origin, iPoint destination);
	~CameraTranslation();

	void Entering();
	void Action();
	void Exiting();

	void SetOriginAndDestination(iPoint origin, iPoint destination);

};

