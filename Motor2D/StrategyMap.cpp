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

	encounter_tree = new EncounterTree();
	encounter_tree->CreateTree();
	current_node = encounter_tree->GetNodes().front();

	App->render->camera.x = 0;
	App->render->camera.y = 0;

	for (int i = 0; i < current_node->GetChildren().size(); i++)
	{
		current_node->GetChildren()[i]->GetEntity()->SetInRange(true);
	}

	current_node->GetEntity()->SetInRange(true);

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
		App->transition_manager->CreateFadeTransition(3.0f, true, 3, White);
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

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	for each (EncounterNode* n in encounter_tree->GetNodes())
	{
		if (n->GetChildren().size() != 0)
		{
			if (n != current_node)
			{
				for (int i = 0; i < n->GetChildren().size(); i++)
				{
					App->render->DrawLine(n->GetPosition().x, n->GetPosition().y, n->GetChildren()[i]->GetPosition().x, n->GetChildren()[i]->GetPosition().y, 255, 0, 0);
				}
			}
			else {
				for (int i = 0; i < n->GetChildren().size(); i++)
				{
					App->render->DrawLine(n->GetPosition().x, n->GetPosition().y, n->GetChildren()[i]->GetPosition().x, n->GetChildren()[i]->GetPosition().y, 0, 255, 0);
				}
			}
		}
	}

	return ret;
}

// Called before quitting
bool StrategyMap::CleanUp()
{
	LOG("Freeing scene");

	App->tex->UnLoad(background);
	encounter_tree->CleanTree();
	App->entity_manager->CleanUp();

	return true;
}

bool StrategyMap::GUIEvent(UIElement * element, GUI_Event gui_event)
{
	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {

		for (int i = 0; i < current_node->GetChildren().size(); i++)
		{
			if (element == current_node->GetChildren()[i]->GetButton())
			{
				App->transition_manager->CreateFadeTransition(3.0f, true, 3, White);
				App->transition_manager->CreateZoomTransition(3.0f);
			}
		}
	}

	return true;
}
