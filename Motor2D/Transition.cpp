#include "Transition.h"
#include "TransitionManager.h"
#include "SceneManager.h"
#include "j1App.h"
#include "Render.h"
#include "Window.h"
#include "p2Log.h"


Transition::Transition()
{
}

Transition::Transition(TransitionType type, float time, bool is_scene_change, int scene_to_change)
{
	this->type = type;
	transition_time = time;
	this->is_scene_change = is_scene_change;
	this->scene_to_change = scene_to_change;

	OnCreate();
}


Transition::~Transition()
{
	delete current_time;
}

void Transition::OnCreate()
{
	uint width, height;

	App->win->GetWindowSize(width, height);
	screen = { 0, 0, (int)width, (int)height};
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	//start timer
	current_time = new Timer();
	current_time->Start();

	state = TransitionState::ENTERING;
}

void Transition::PreUpdate()
{
}

void Transition::Update()
{

}

void Transition::PostUpdate()
{
	switch (state)
	{
	case Transition::TransitionState::NONE:
		break;
	case Transition::TransitionState::ENTERING:
		Entering();
		break;
	case Transition::TransitionState::ACTION:
		Action();
		break;
	case Transition::TransitionState::EXITING:
		Exiting();
		break;
	default:
		break;
	}
}

Transition::TransitionState Transition::GetState()
{
	return TransitionState();
}

void Transition::SetState(TransitionState state)
{
}

Transition::TransitionType Transition::GetType()
{
	return TransitionType();
}

void Transition::SetColor(Color color)
{
	this->color = color;
}

void Transition::Entering()
{
	switch (type)
	{
	case Transition::TransitionType::FADE: {

		float normalized_alpha = floor(current_time->ReadSec()*(255.0F / transition_time));
		DrawFadeRect(normalized_alpha);

		if (current_time->ReadSec() >= transition_time)
		{
			state = TransitionState::ACTION;
		}
	}break;

	case Transition::TransitionType::ZOOM: {
		current_scale += scale_increment;
		
		if (current_scale >= target_scale) 
		{
			state = TransitionState::ACTION;
		}
	}
		break;
	default:
		break;
	}
}

void Transition::Action()
{
	current_time->Stop();

	switch (type)
	{
	case Transition::TransitionType::FADE:
		DrawFadeRect(255.0F);
		break;
	case Transition::TransitionType::ZOOM:
		break;
	default:
		break;
	}

	if (is_scene_change)
	{
		App->scene_manager->ChangeScene(scene_to_change);
	}

	transition_time += transition_time;
	state = TransitionState::EXITING;
}

void Transition::Exiting()
{
	current_time->Resume();

	switch (type)
	{
	case Transition::TransitionType::FADE: {

		float normalized_alpha = floor((transition_time - current_time->ReadSec())*(255.0F / transition_time));
		DrawFadeRect(normalized_alpha);

		if (current_time->ReadSec() >= transition_time)
		{
			state = TransitionState::NONE;
			App->transition_manager->DestroyTransition(this);
		}
	}break;

	case Transition::TransitionType::ZOOM:
		current_scale -= scale_increment;

		break;
	default:
		break;
	}

}

void Transition::DrawFadeRect(float alpha_value)
{
	if (alpha_value > 255)alpha_value = 255;
	if (alpha_value < 0)alpha_value = 0;

	SDL_SetRenderDrawColor(App->render->renderer, color.r, color.g, color.b, alpha_value);
	SDL_RenderFillRect(App->render->renderer, &screen);
}

void Transition::SetScale(int scale)
{
	target_scale = scale;
}
