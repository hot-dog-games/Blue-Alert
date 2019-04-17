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
	text_menu = App->gui->CreateLabel({ 50,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "MENU", { 0,0,0,0 }, 0, menu_button);

	gold = App->gui->CreateLabel({ 90, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "GOLD", { 0,0,0,0 }, 0, main_panel);
	energy = App->gui->CreateLabel({ 450, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "ENERGY", { 0,0,0,0 },0, main_panel);
	health = App->gui->CreateLabel({ 860, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "HEALTH", { 0,0,0,0 }, 0, main_panel);

	iPoint world_position = App->map->MapToWorld((int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().x, (int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().y);
	
	App->render->camera.x = -world_position.x + w * 0.5;
	App->render->camera.y = -world_position.y + h;

	t_b_bg = App->gui->CreateImage({ 20,95 }, { 26,908,986,593 }, main_panel);
	App->gui->DisableElement(t_b_bg);
	backbutton_t_b = App->gui->CreateButton({ 940,100 }, small_button_rect, main_panel);
	back_cross_text = App->gui->CreateLabel({ 15,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 28, "X", { 0,0,0,0 }, 0, backbutton_t_b);
	App->gui->DisableElement(backbutton_t_b);
	troops_button = App->gui->CreateButton({380,100 }, large_button_rect, main_panel);
	troops_text = App->gui->CreateLabel({50,10}, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "TROOPS", { 0,0,0,0 }, 0, troops_button);
	App->gui->DisableElement(troops_button);
	buildings_button = App->gui->CreateButton({ 600,100 }, large_button_rect, main_panel);
	buildings_text = App->gui->CreateLabel({ 35,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "BUILDINGS", { 0,0,0,0 }, 0, buildings_button);
	App->gui->DisableElement(buildings_button);


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
	

	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {
		if (element == settings_button) {
			//(options = App->gui->CreateImage();
		}
		else if (element == menu_button) {
			App->gui->EnableElement(troops_button);
			App->gui->EnableElement(buildings_button);
			App->gui->EnableElement(t_b_bg);
			App->gui->EnableElement(backbutton_t_b);

			App->gui->DisableElement(menu_button);
			App->gui->DisableElement(settings_button);



		}
		else if (element = backbutton_t_b) {
			App->gui->DisableElement(t_b_bg);
			App->gui->DisableElement(backbutton_t_b);
			App->gui->DisableElement(troops_button);
			App->gui->DisableElement(buildings_button);

			App->gui->EnableElement(menu_button);
			App->gui->EnableElement(settings_button);

		}
	}

	return true;
}
