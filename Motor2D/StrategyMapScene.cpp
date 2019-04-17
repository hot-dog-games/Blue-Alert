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
#include "StrategyMapScene.h"
#include "Brofiler/Brofiler.h"

StrategyMapScene::StrategyMapScene() : Scene()
{

}

// Destructor
StrategyMapScene::~StrategyMapScene()
{}

// Called before the first frame
bool StrategyMapScene::Start()
{
	BROFILER_CATEGORY("SMStart", Profiler::Color::Red);

	App->map->Load("Nodes Map.tmx");
	App->ResumeGame();

	App->game_manager->GetEncounterTree()->CreateAllNodes();
	App->game_manager->GetEncounterTree()->UpdateTreeState();

	//UI
	uint w, h;
	App->win->GetWindowSize(w, h);

	main_panel = App->gui->CreateImage({ 0,0 }, { 0, 0, (int)w, (int)h }, nullptr, false);
	banner = App->gui->CreateImage({ 4,5 }, { 1,769,1017,83 }, main_panel);
	SDL_Rect small_button_rect[3];
	small_button_rect[0] = { 0,481,59,51 };
	small_button_rect[1] = { 60,481,58,51 };
	small_button_rect[2] = { 119,481,58,51 };

	SDL_Rect large_button_rect[3];
	large_button_rect[0] = { 0,533,220,51 };
	large_button_rect[1] = { 0,585,220,51 };
	large_button_rect[2] = { 0,637,220,51 };

	settings_button = App->gui->CreateButton({ 50,700 }, small_button_rect, main_panel);
	menu_button = App->gui->CreateButton({ 700,700 }, large_button_rect, main_panel);

	gold = App->gui->CreateLabel({ 90, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "GOLD", { 0,0,0,0 }, 0, main_panel);
	energy = App->gui->CreateLabel({ 450, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "ENERGY", { 0,0,0,0 },0, main_panel);
	health = App->gui->CreateLabel({ 860, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "HEALTH", { 0,0,0,0 }, 0, main_panel);

	iPoint world_position = App->map->MapToWorld((int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().x, (int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().y);
	
	App->render->camera.x = -world_position.x + w * 0.5;
	App->render->camera.y = -world_position.y + h;


	return true;
}

// Called each loop iteration
bool StrategyMapScene::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 10;
	return true;
}

// Called each loop iteration
bool StrategyMapScene::Update(float dt)
{
	BROFILER_CATEGORY("SMUpdate", Profiler::Color::Green);
	return true;
}

// Called each loop iteration
bool StrategyMapScene::PostUpdate()
{
	bool ret = true;

	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	App->game_manager->GetEncounterTree()->DrawTreeLines();

	return ret;
}

// Called before quitting
bool StrategyMapScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool StrategyMapScene::GUIEvent(UIElement * element, GUI_Event gui_event)
{
	SDL_Rect small_button_rect[3];
	small_button_rect[0] = { 0,481,59,51 };
	small_button_rect[1] = { 60,481,58,51 };
	small_button_rect[2] = { 119,481,58,51 };

	SDL_Rect large_button_rect[3];
	large_button_rect[0] = { 0,533,220,51 };
	large_button_rect[1] = { 0,585,220,51 };
	large_button_rect[2] = { 0,637,220,51 };

	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {

	}

	return true;
}
