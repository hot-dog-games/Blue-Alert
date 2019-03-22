#include "Zoom.h"
#include "j1App.h"
#include "Render.h"
#include "Window.h"
#include "p2Log.h"


Zoom::Zoom(float transition_time, float target_scale) : Transition(transition_time)
{
	this->target_scale = target_scale;
}

Zoom::~Zoom()
{
}

void Zoom::Entering()
{
	Transition::Entering();

	float normalized_scale = current_time->ReadSec()*(target_scale / transition_time) + normal_scale;
	float scale_increment = normalized_scale - current_scale;

	App->render->camera.x -= scale_increment * ((App->render->camera.w*0.5f) / current_scale);
	App->render->camera.y -= scale_increment * ((App->render->camera.h*0.5f) / current_scale);

	current_scale = normalized_scale;
	SDL_RenderSetScale(App->render->renderer, normalized_scale, normalized_scale);

	LOG("scale increment %f", scale_increment);
}

void Zoom::Exiting()
{
	Transition::Exiting();

	current_scale = normal_scale;
	SDL_RenderSetScale(App->render->renderer, normal_scale, normal_scale);
}

void Zoom::SetTargetScale(int target_scale)
{
	this->target_scale = target_scale;
}