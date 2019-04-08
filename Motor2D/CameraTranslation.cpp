#include "CameraTranslation.h"
#include "j1App.h"
#include "Render.h"
#include "p2Log.h"


CameraTranslation::CameraTranslation(float transition_time, iPoint origin, iPoint destination) : Transition(transition_time)
{
	this->origin = origin;
	this->destination = destination;

	distance = origin.DistanceTo(destination);
}

CameraTranslation::~CameraTranslation()
{

}

void CameraTranslation::Entering()
{
	Transition::Entering();

	float percent = current_time->ReadSec()*(1 / transition_time);

	LOG("percent %f", percent);

	float step_x = percent * distance;
	float step_y = percent * distance;

	App->render->camera.x = step_x;
	App->render->camera.y = step_y;
}

void CameraTranslation::SetOriginAndDestination(iPoint origin, iPoint destination)
{
	this->origin = origin;
	this->destination = destination;
}
