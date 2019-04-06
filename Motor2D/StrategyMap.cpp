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

	//UI
	uint w, h;
	App->win->GetWindowSize(w, h);

	main_panel = App->gui->CreateImage({ 0,0 }, { 0, 0, (int)w, (int)h }, nullptr, false);
	banner = App->gui->CreateImage({ 4,5 }, { 1,769,1017,83 }, main_panel);
	SDL_Rect settings_rect[3];
	settings_rect[0] = { 0,481,59,51 };
	settings_rect[1] = { 60,481,58,51 };
	settings_rect[2] = { 119,481,58,51 };

	SDL_Rect menu_rect[3];
	menu_rect[0] = { 0,533,220,51 };
	menu_rect[1] = { 0,585,220,51 };
	menu_rect[2] = { 0,585,220,51 };

	settings_button = App->gui->CreateButton({ 50,700 }, settings_rect, main_panel);
	menu_button = App->gui->CreateButton({ 700,700 }, menu_rect, main_panel);

	gold = App->gui->CreateLabel({ 90, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "GOLD", { 0,0,0,0 }, 0, main_panel);
	energy = App->gui->CreateLabel({ 450, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "ENERGY", { 0,0,0,0 },0, main_panel);
	health = App->gui->CreateLabel({ 860, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "HEALTH", { 0,0,0,0 }, 0, main_panel);

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
		App->transition_manager->CreateFadeTransition(2.0f, true, 3, White);
		App->transition_manager->CreateZoomTransition(2.0f);
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
				App->gui->DisableElement(main_panel);
				App->transition_manager->CreateFadeTransition(2.0f, true, 3, White);
				App->transition_manager->CreateZoomTransition(2.0f);
			}
		}
	}

	return true;
}
