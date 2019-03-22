#include "CameraTranslation.h"
#include "j1App.h"
#include "Render.h"
#include "p2Log.h"


CameraTranslation::CameraTranslation(float transition_time, iPoint origin, iPoint destination) : Transition(transition_time)
{
	this->origin = origin;
	this->destination = destination;

	distance = origin.DistanceManhattan(destination);
}

CameraTranslation::~CameraTranslation()
{

}

void CameraTranslation::Entering()
{
	Transition::Entering();

	float percent = current_time->ReadSec()*(1 / transition_time);

	float step_x = origin.x + percent * distance;
	float step_y = origin.y + percent * distance;

	App->render->camera.x = step_x;
	App->render->camera.y = step_y;
}

void CameraTranslation::SetOriginAndDestination(iPoint origin, iPoint destination)
{
	this->origin = origin;
	this->destination = destination;
}
