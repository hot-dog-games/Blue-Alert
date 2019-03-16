#include "Transition.h"
#include "TransitionManager.h"
#include "j1App.h"
#include "Render.h"
#include "Window.h"


Transition::Transition()
{
}

Transition::Transition(TransitionType type, float time, bool is_scene_change, int scene_to_change)
{
	m_type = type;
	transition_time = time;
	this->is_scene_change = is_scene_change;
	this->scene_to_change = scene_to_change;

	m_state = TransitionState::ENTERING;
}


Transition::~Transition()
{
}

void Transition::OnCreate()
{
	uint width, height;

	App->win->GetWindowSize(width, height);
	screen = { 0, 0, (int)width, (int)height};
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	current_time->Start();
}

void Transition::PreUpdate()
{
}

void Transition::Update()
{
	switch (m_state)
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

void Transition::PostUpdate()
{
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

void Transition::Entering()
{
	switch (m_type)
	{
	case Transition::TransitionType::FADE:

		break;
	case Transition::TransitionType::ZOOM:
		break;
	default:
		break;
	}
}

void Transition::Action()
{
}

void Transition::Exiting()
{
}

void Transition::DrawFadeRect()
{
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, current_time->ReadSec()*255.0f);
	SDL_RenderFillRect(App->render->renderer, &screen);
}
