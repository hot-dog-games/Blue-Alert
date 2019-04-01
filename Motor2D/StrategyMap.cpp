#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "EntityManager.h"
#include "TransitionManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "StrategyMap.h"

StrategyMap::StrategyMap() : Scene()
{

}

// Destructor
StrategyMap::~StrategyMap()
{}

// Called before the first frame
bool StrategyMap::Start()
{
	level = new EncounterTree();
	level->CreateTree();

	background = App->tex->Load("textures/strategy_scene/background.png");

	return true;
}

// Called each loop iteration
bool StrategyMap::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool StrategyMap::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) {
		App->transition_manager->CreateFadeTransition(3.0f, false, 0, White);
		App->transition_manager->CreateZoomTransition(3.0f);
		//App->transition_manager->CreateCameraTranslation(3.0f, { App->render->camera.x, App->render->camera.y }, { 0, 0 });
	}


	return true;
}

// Called each loop iteration
bool StrategyMap::PostUpdate()
{
	bool ret = true;

	App->map->Draw();
	App->render->Blit(background, 0, 0);
	App->render->camera.y = -300;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool StrategyMap::CleanUp()
{
	LOG("Freeing scene");

	App->tex->UnLoad(background);

	return true;
}