#include "TransitionManager.h"
#include "j1App.h"
#include "Render.h"
#include "Window.h"
#include "p2Log.h"

TransitionManager::TransitionManager()
{

}


TransitionManager::~TransitionManager()
{

}

bool TransitionManager::Awake(pugi::xml_node &)
{
	LOG("Awakening Transition Manager");
	return true;
}

bool TransitionManager::Start()
{
	LOG("Starting Transition Manager");

	uint width = 0U;
	uint height = 0U;

	App->win->GetWindowSize(width, height);

	screen = { 0, 0, (int)width, (int)height };
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);

	return true;
}

bool TransitionManager::PreUpdate()
{
	//TODO
	return true;
}

bool TransitionManager::Update(float dt)
{
	switch (state)
	{
	case TransitionState::ENTERING:
	{

	} break;

	case TransitionState::EXITING:
	{

	} break;

	case TransitionState::NONE:
	{

	} break;
	}

	return true;
}

bool TransitionManager::PostUpdate()
{
	switch (type)
	{
	case TransitionType::FADE: {


	}break;
	}

	return true;
}

bool TransitionManager::CleanUp()
{
	//TODO
	return true;
}

void TransitionManager::ChangeSceneWithFade(Timer time)
{
	state = TransitionState::ENTERING;
	current_time->Start();
	transition_time = time.ReadSec();
}