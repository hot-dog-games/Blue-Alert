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
#include "GUI.h"
#include "EncounterNode.h"
#include "StrategyBuilding.h"
#include "GameManager.h"
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
	App->map->Load("strategy_map.tmx");

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	App->game_manager->GetEncounterTree()->UpdateTreeState();

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
	return true;
}

// Called each loop iteration
bool StrategyMap::PostUpdate()
{
	bool ret = true;

	App->map->Draw();
	App->render->Blit(background, 0, 0);

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	App->game_manager->GetEncounterTree()->DrawTreeLines();

	return ret;
}

// Called before quitting
bool StrategyMap::CleanUp()
{
	LOG("Freeing scene");

	App->tex->UnLoad(background);
	App->entity_manager->CleanUp();

	return true;
}

bool StrategyMap::GUIEvent(UIElement * element, GUI_Event gui_event)
{
	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {

		for (int i = 0; i < App->game_manager->GetEncounterTree()->GetCurrentNode()->GetChildren().size(); i++)
		{
			if (element == App->game_manager->GetEncounterTree()->GetCurrentNode()->GetChildren()[i]->GetButton())
			{
				App->transition_manager->CreateFadeTransition(3.0f, true, 3, White);
				App->transition_manager->CreateZoomTransition(3.0f);
				App->game_manager->GetEncounterTree()->SetCurrentNode(App->game_manager->GetEncounterTree()->GetCurrentNode()->GetChildren()[i]);
			}
		}

	}

	return true;
}
