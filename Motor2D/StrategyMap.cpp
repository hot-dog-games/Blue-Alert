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
#include "GUI.h"

StrategyMap::StrategyMap() : Scene()
{

}

// Destructor
StrategyMap::~StrategyMap()
{}

// Called before the first frame
bool StrategyMap::Start()
{
	encounter_tree = new EncounterTree();
	encounter_tree->CreateTree();
	current_node = encounter_tree->GetNodes().front();

	background = App->tex->Load("textures/strategy_scene/background.png");
	node_rect = { 600, 420, 47, 57 };

	CreateNodeButtons();

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

	return true;
}

void StrategyMap::CreateNodeButtons()
{
	for each (EncounterNode* n in encounter_tree->GetNodes())
	{
		node_buttons.push_back(App->gui->CreateButton(n->GetPosition(), &node_rect));
	}
}

bool StrategyMap::GUIEvent(UIElement * element, GUI_Event gui_event)
{
	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {
		if (element == node_buttons[2])
		{
			App->transition_manager->CreateFadeTransition(3.0f, false, 0, White);
			App->transition_manager->CreateZoomTransition(3.0f);
		}
	}

	return true;
}
