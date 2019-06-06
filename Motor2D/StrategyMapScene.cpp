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
#include "UISelectableButton.h"
#include "UILabel.h"
#include "UIButton.h"
#include "UIButtonTroops.h"
#include "Brofiler/Brofiler.h"
#include "Stat.h"
#include "UIBar.h"
#include "SceneManager.h"
#include "BuffSourceManager.h"

#include <stdio.h>
#include <string>

StrategyMapScene::StrategyMapScene() : Scene()
{

}

// Destructor
StrategyMapScene::~StrategyMapScene()
{}

// Called before the first frame
bool StrategyMapScene::Start()
{
	if(App->game_manager->restart)
		App->game_manager->Restart();
	else if (App->game_manager->change_stage)
		App->game_manager->ChangeStage();

	App->SaveGame(nullptr);

	BROFILER_CATEGORY("SMStart", Profiler::Color::Red);

	switch (App->game_manager->stage)
	{
	case STAGE_TUTORIAL: 
		App->map->Load("Tutorial_Nodes_Map.tmx"); 
		map_camera_limit = { 0, 0, (int)(2000 * App->win->GetScale()), (int)(1692 * App->win->GetScale()) };
		break;
	case STAGE_01: 
		App->map->Load("Nodes Map.tmx");
		map_camera_limit = { 0, 0, (int)(2600 * App->win->GetScale()), (int)(1792 * App->win->GetScale()) };
		break;
	case STAGE_02: 
		App->map->Load("Nodes Map Snow.tmx"); 
		map_camera_limit = { 0, 0, (int)(2600 * App->win->GetScale()), (int)(1992 * App->win->GetScale()) };
		break;
	default:
		break;
	}
	App->ResumeGame();

	App->game_manager->GetEncounterTree()->CreateAllNodes();
	App->game_manager->GetEncounterTree()->UpdateTreeState();

	uint w, h;
	App->win->GetWindowSize(w, h);

	map_camera_limit.x -= (map_camera_limit.w * 0.5);
	map_camera_limit.y = ((App->map->data.height*0.5*App->map->data.tile_height) * App->win->GetScale()) - map_camera_limit.h*0.5;

	iPoint world_position = App->map->MapToWorld((int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().x, (int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().y);

	App->render->camera.x = -((world_position.x * App->win->GetScale()) - w * 0.5);
	App->render->camera.y = -((world_position.y * App->win->GetScale()) - h * 0.9);

	InitializeUI();
	
	App->audio->PlayMusic("audio/music/5.InDeep-RedAlert2_2.ogg");


	return true;
}

// Called each loop iteration
bool StrategyMapScene::PreUpdate()
{
	int mousemotion_x, mousemotion_y;

	App->input->GetMouseMotion(mousemotion_x, mousemotion_y);

	if (dragable) {

		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
		{

			if (abs(mousemotion_x) > drag_threshhold && abs(mousemotion_y) > drag_threshhold)
			{
				App->render->camera.x += mousemotion_x;
				App->render->camera.y += mousemotion_y;
			}
		
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 10;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 10;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 10;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 10;


	KeepInBounds();

	last_camera_position.x = App->render->camera.x;
	last_camera_position.y = App->render->camera.y;

	return true;
}

// Called each loop iteration
bool StrategyMapScene::Update(float dt)
{	
	if (!App->game_manager->popups[POPUP_DECISIONMAKING])
	{
		if (App->game_manager->GetEncounterTree()->GetCurrentNode()->GetChildren().size() > 1)
		{
			if (!App->transition_manager->IsTransitioning())
				App->game_manager->ShowPopUp(POPUP_DECISIONMAKING);
		}
	};

	if (!App->game_manager->popups[POPUP_TUTORIAL_END])
	{
		if (App->game_manager->stage == STAGE_01)
		{
			if (!App->transition_manager->IsTransitioning())
				App->game_manager->ShowPopUp(POPUP_TUTORIAL_END);
		}
	};

	if (App->gui->popup_active)
		dragable = false;

	return true;
}

// Called each loop iteration
bool StrategyMapScene::PostUpdate()
{
	BROFILER_CATEGORY("SMPostUpdate", Profiler::Color::Green);
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
			App->gui->EnableElement(options);

			App->gui->DisableElement(collection_button);
			App->gui->DisableElement(settings_button);

			App->game_manager->GetEncounterTree()->is_clickable = false;
			dragable = false;

		}
		else if (element == resume_settings_button) {
			App->gui->DisableElement(options);

			App->gui->EnableElement(collection_button);
			App->gui->EnableElement(settings_button);
			App->game_manager->GetEncounterTree()->is_clickable = true;
			dragable = true;
		}

		else if (element == collection_button || (element == troops_button && !troops_background->IsEnabled())) {
			
			App->gui->EnableElement(troops_button);
			App->gui->EnableElement(buildings_button);
			App->gui->EnableElement(troops_background);
			App->gui->EnableElement(backbutton_t_b);

			for (int i = 0; i < 9; ++i) {
				App->gui->DisableElement(collection_buttons_enemies[i]);
			}

			App->gui->DisableElement(collection_button);
			App->gui->DisableElement(settings_button);
			App->gui->DisableElement(buildings_background);

			App->game_manager->GetEncounterTree()->is_clickable = false;
			dragable = false;

			for (uint i = 0; i < 4; ++i) {
				if(!App->game_manager->GetPlayerDeck()->cards[i])
					App->gui->DisableElement(deck_buttons[i]);
			}

			building_title->SetText("Infantry Building:");

			troops_button->SetLocked(false);
			buildings_button->SetLocked(true);

		}
		else if (element == backbutton_t_b) {
			App->gui->DisableElement(troops_background);
			App->gui->DisableElement(buildings_background);
			App->gui->DisableElement(backbutton_t_b);
			App->gui->DisableElement(troops_button);
			App->gui->DisableElement(buildings_button);

			App->gui->EnableElement(collection_button);
			App->gui->EnableElement(settings_button);

			App->game_manager->GetEncounterTree()->is_clickable = true;
			dragable = true;

		}
		else if (element == buildings_button) {
			App->gui->DisableElement(troops_background);
			App->gui->EnableElement(buildings_background);
			App->gui->DisableElement(building_aerial_image);
			App->gui->DisableElement(building_land_image);
			building_infantry_button->ChangeState(true);
			building_aerial_button->ChangeState(false);
			building_land_button->ChangeState(false);
			building_infantry_button->interactable = false;
			building_aerial_button->interactable = true;
			building_land_button->interactable = true;

			troops_button->SetLocked(true);
		
			buildings_button->SetLocked(false);

		}
		else if (element == change_side_button) {
			if (collection_buttons_allies[0]->IsEnabled()) {
				for (int i = 0; i < 9; ++i) {
					App->gui->EnableElement(collection_buttons_enemies[i]);
					App->gui->DisableElement(collection_buttons_allies[i]);
					side_label->SetText("Enemies");
					side_label->SetColor({ 25,68,160,255 });
				}
			}
			else {
				for (int i = 0; i < 9; ++i) {
					App->gui->EnableElement(collection_buttons_allies[i]);
					App->gui->DisableElement(collection_buttons_enemies[i]);
					side_label->SetText("Allies");
					side_label->SetColor({ 160,25,25,255 });
				}
			}
		}
		else if (element == deck_buttons[0] && App->game_manager->GetPlayerDeck()->GetDeckSize() > 1)
		{
			App->gui->DisableElement(deck_buttons[0]);
			App->game_manager->GetPlayerDeck()->RemoveCard(0);
		}
		else if (element == deck_buttons[1] && App->game_manager->GetPlayerDeck()->GetDeckSize() > 1)
		{
			App->gui->DisableElement(deck_buttons[1]);
			App->game_manager->GetPlayerDeck()->RemoveCard(1);
		}
		else if (element == deck_buttons[2] && App->game_manager->GetPlayerDeck()->GetDeckSize() > 1)
		{
			App->gui->DisableElement(deck_buttons[2]);
			App->game_manager->GetPlayerDeck()->RemoveCard(2);
		}
		else if (element == deck_buttons[3] && App->game_manager->GetPlayerDeck()->GetDeckSize() > 1)
		{
			App->gui->DisableElement(deck_buttons[3]);
			App->game_manager->GetPlayerDeck()->RemoveCard(3);
		}
		if (element == collection_buttons_allies[0])
		{
			AddCardToDeck(element,2);
		}
		else if (element == collection_buttons_allies[1])
		{
			AddCardToDeck(element,4);
		}
		else if (element == collection_buttons_allies[2])
		{
			AddCardToDeck(element,6);
		}
		else if (element == collection_buttons_allies[3])
		{
			AddCardToDeck(element,8);
		}
		else if (element == collection_buttons_allies[4])
		{
			AddCardToDeck(element,10);
		}
		else if (element == collection_buttons_allies[5])
		{
			AddCardToDeck(element,12);
		}
		else if (element == collection_buttons_allies[6])
		{
			AddCardToDeck(element,14);
		}
		else if (element == collection_buttons_allies[7])
		{
			AddCardToDeck(element,16);
		}
		else if (element == collection_buttons_allies[8])
		{
			AddCardToDeck(element,18);
		}
		if (element == collection_buttons_enemies[0])
		{
		AddCardToDeck(element, 1);
		}
		else if (element == collection_buttons_enemies[1])
		{
		AddCardToDeck(element, 3);
		}
		else if (element == collection_buttons_enemies[2])
		{
		AddCardToDeck(element, 5);
		}
		else if (element == collection_buttons_enemies[3])
		{
		AddCardToDeck(element, 7);
		}
		else if (element == collection_buttons_enemies[4])
		{
		AddCardToDeck(element, 9);
		}
		else if (element == collection_buttons_enemies[5])
		{
		AddCardToDeck(element, 11);
		}
		else if (element == collection_buttons_enemies[6])
		{
		AddCardToDeck(element, 13);
		}
		else if (element == collection_buttons_enemies[7])
		{
		AddCardToDeck(element, 15);
		}
		else if (element == collection_buttons_enemies[8])
		{
		AddCardToDeck(element, 17);
		}

		if (element == core_lvl_up_health) {
			uint cost = ((LeveledUpgrade*)App->game_manager->health_upgrade)->GetCost();
			if (App->game_manager->gold >= cost)
			{
				App->game_manager->gold -= cost;
				App->game_manager->UpgradeHealth();
				gold_quantity->SetText("Gold: " + std::to_string(App->game_manager->gold) + "g");

				std::string str = "";

				str = "Health: " + std::to_string((int)App->game_manager->stats.find("health")->second->GetValue());
				core_health->SetText(str);

				cost = ((LeveledUpgrade*)App->game_manager->health_upgrade)->GetCost();
				str = cost > 0 ? "Cost Health: " + std::to_string(cost) : "Maxed";
				core_lvl_up_health_cost->SetText(str);
				if (cost == 0)
					core_lvl_up_health->SetLocked(false);
			}
		}
		else if (element == core_lvl_up_energy) {
			uint cost = ((LeveledUpgrade*)App->game_manager->energy_upgrade)->GetCost();
			if (App->game_manager->gold >= cost)
			{
				App->game_manager->gold -= cost;
				App->game_manager->UpgradeEnergy();
				gold_quantity->SetText("Gold: " + std::to_string(App->game_manager->gold));

				std::string str = "";

				str = "Energy: " + std::to_string((int)App->game_manager->stats.find("energy")->second->GetValue());
				core_energy->SetText(str);

				cost = ((LeveledUpgrade*)App->game_manager->energy_upgrade)->GetCost();
				str = cost > 0 ? "Cost Energy: " + std::to_string(cost) : "Maxed";
				core_lvl_up_energy_cost->SetText(str);
				if (cost == 0)
					core_lvl_up_energy->SetLocked(false);
			}
		}

		// Building butttons
		if (element == building_infantry_button) {
			if (building_infantry_button->IsSelected()) {
				App->gui->EnableElement(building_infantry_image);
				App->gui->DisableElement(building_aerial_image);
				App->gui->DisableElement(building_land_image);
				building_aerial_button->ChangeState(false);
				building_land_button->ChangeState(false);
				building_title->SetText("Infantry Building:");
				building_infantry_button->interactable = false;
				building_aerial_button->interactable = true;
				building_land_button->interactable = true;
			}
		}
		else if (element == building_aerial_button) {
			if (building_aerial_button->IsSelected()) {
				App->gui->EnableElement(building_aerial_image);
				App->gui->DisableElement(building_infantry_image);
				App->gui->DisableElement(building_land_image);
				building_infantry_button->ChangeState(false);
				building_land_button->ChangeState(false);
				building_title->SetText("Aerial Building:");
				building_aerial_button->interactable = false;
				building_infantry_button->interactable = true;
				building_land_button->interactable = true;
			}
			else {
				App->gui->DisableElement(building_aerial_image);
				building_title->SetText("");
			}
		}
		else if (element == building_land_button) {
			if (building_land_button->IsSelected()) {
				App->gui->EnableElement(building_land_image);
				App->gui->DisableElement(building_infantry_image);
				App->gui->DisableElement(building_aerial_image);
				building_infantry_button->ChangeState(false);
				building_aerial_button->ChangeState(false);
				building_title->SetText("Land Building:");
				building_land_button->interactable = false;
				building_infantry_button->interactable = true;
				building_aerial_button->interactable = true;
			}
			else {
				App->gui->DisableElement(building_land_image);
				building_title->SetText("");
			}
		}

		if (element == back_menu_button) {
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MENU, Black);
		}
	}
	else if (gui_event == GUI_Event::MOUSE_OVER)
	{
	std::string str_stat = "";
		for (int num = 0; num < 9; ++num) {
			if (element == collection_buttons_allies[num]) {
				info_image->SetImage(collection_buttons_allies[num]->GetAnim()[0]);
				str_stat = "Health: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 2))->info.stats.find("health")->second->GetValue());
				health_label->SetText(str_stat);
				str_stat = "Attack: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 2))->info.stats.find("damage")->second->GetValue());
				attack_label->SetText(str_stat);
				str_stat = "Defense: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 2))->info.stats.find("defense")->second->GetValue());
				defense_label->SetText(str_stat);
				str_stat = "Range: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 2))->info.stats.find("range")->second->GetValue());
				range_label->SetText(str_stat);
				str_stat = "Units: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 2))->info.stats.find("units")->second->GetValue());
				units_label->SetText(str_stat);
				energy_bar->ChangeStat(App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 2))->info.stats.find("energy_cost")->second);
			}
			else if(element == collection_buttons_enemies[num]) {
				info_image->SetImage(collection_buttons_enemies[num]->GetAnim()[0]);
				str_stat = "Health: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 1))->info.stats.find("health")->second->GetValue());
				health_label->SetText(str_stat);
				str_stat = "Attack: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 1))->info.stats.find("damage")->second->GetValue());
				attack_label->SetText(str_stat);
				str_stat = "Defense: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 1))->info.stats.find("defense")->second->GetValue());
				defense_label->SetText(str_stat);
				str_stat = "Range: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 1))->info.stats.find("range")->second->GetValue());
				range_label->SetText(str_stat);
				str_stat = "Units: " + std::to_string((int)App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 1))->info.stats.find("units")->second->GetValue());
				units_label->SetText(str_stat);
				energy_bar->ChangeStat(App->game_manager->GetCardFromCollection((EntityType)((num * 2) + 1))->info.stats.find("energy_cost")->second);
			}
		}
	}

	return true;
}

void  StrategyMapScene::AddCardToDeck(UIElement * element, uint num) {
	for (int i = 0; i < 4; i++) {
		Card* card = App->game_manager->GetCardFromCollection((EntityType)(num));
		if (deck_buttons[i] && deck_buttons[i]->enabled == false && card && !App->game_manager->IsInPlayerDeck(card)) {
			App->gui->EnableElement(deck_buttons[i]);
			App->game_manager->GetPlayerDeck()->AddCard(card);
			deck_buttons[i]->ChangeSprite(App->gui->LoadUIButton((App->game_manager->GetPlayerDeck()->cards[i]->type), "upgrade"));
			break;
		}
		else if (!deck_buttons[i] && card && !App->game_manager->IsInPlayerDeck(card)) {
			App->game_manager->GetPlayerDeck()->AddCard(card);
			deck_buttons[i] = App->gui->CreateButton({ (int)(360 + i * 140), 99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[i]->type, "upgrade"), troops_background);
			App->gui->EnableElement(deck_buttons[i]);
			deck_buttons[i]->ChangeSprite(App->gui->LoadUIButton((App->game_manager->GetPlayerDeck()->cards[i]->type), "upgrade"));
			break;
		}
	}
}

void StrategyMapScene::InitializeUI()
{
	uint w, h;
	App->win->GetWindowSize(w, h);

	main_panel = App->gui->CreateImage({ 0,0 }, { 0, 0, 0, 0 }, nullptr, false);

	SDL_Rect small_button_rect[3];
	small_button_rect[0] = { 753, 497, 41, 40 };
	small_button_rect[1] = { 753, 538, 41, 40 };
	small_button_rect[2] = { 753, 577, 41, 40 };

	SDL_Rect big_button_rect[4];
	big_button_rect[0] = { 447,495,303,76 };
	big_button_rect[1] = { 447,574,303,76 };
	big_button_rect[2] = { 447,650,303,76 };
	big_button_rect[3] = { 447,729,303,76 };

	SDL_Rect medium_button_rect[4];
	medium_button_rect[0] = { 800,499,289,66 };
	medium_button_rect[1] = { 800,569,289,66 };
	medium_button_rect[2] = { 800,639,289,66 };
	medium_button_rect[3] = { 800,639,289,66 };

	SDL_Rect little_button_rect[4];
	little_button_rect[0] = { 1256,379,145,67 };
	little_button_rect[1] = { 1256,449,145,67 };
	little_button_rect[2] = { 1256,519,145,67 };
	little_button_rect[3] = { 1256,379,145,67 };

	SDL_Rect change_button[3];
	change_button[0] = { 3,700,53,51 };
	change_button[1] = { 77,700,53,51 };
	change_button[2] = { 148,700,53,51 };

	SDL_Rect options_rect[3];
	options_rect[0] = { 874,1297,81,66 };
	options_rect[1] = { 959,1297,81,66 };
	options_rect[2] = { 1044,1297,81,66 };

	std::string str = "";

	// Map Buttons
	collection_button = App->gui->CreateButtonText({ 336, 880 }, { 10,0 }, medium_button_rect, "COLLECTION", { 200,200,200,255 }, 27, main_panel);
	options = App->gui->CreateImage({ 1,150 }, { 2705,923,637,593 }, nullptr);
	options_label = App->gui->CreateLabel({ 120,34 }, "fonts/button_text.ttf", 60, "Options", { 255,255,255,255 }, 500, options);
	settings_button = App->gui->CreateButton({ 20, 880 }, options_rect, main_panel);
	resume_settings_button = App->gui->CreateButtonText({ 320,480 }, { 80,15 }, medium_button_rect, "Resume", {255,255,255,0}, 20, options);

	music_slider = App->gui->CreateScrollBar({ 350,190 }, { 939,1365,218,40 }, MUSIC, App->audio->GetMusicVolume(), 128, options);
	musiclabel = App->gui->CreateLabel({ 50,200 }, "fonts/button_text.ttf", 20, "Music Volume", { 255,255,255,0 }, 0, options);
	fx_slider = App->gui->CreateScrollBar({ 350,290 }, { 939,1365,218,40 }, FX, App->audio->GetFxVolume(), 128, options);
	fxlabel = App->gui->CreateLabel({ 50,300 }, "fonts/button_text.ttf", 20, "FX Volume", { 255,255,255,0 }, 0, options);
	back_menu_button = App->gui->CreateButtonText({ 30,480 }, { 20,15 }, medium_button_rect, "BACK TO MENU", { 255,255,255,0 }, 20, options);
	App->gui->DisableElement(options);

	// Troops menu
	troops_background = App->gui->CreateImage({ 8, 20 }, { 2705,5,673,911 }, main_panel);
	change_side_button = App->gui->CreateButton({ 564,773 }, change_button, troops_background);
	side_label = App->gui->CreateLabel({ 700, 607 }, "fonts/button_text.ttf", 30, "Allies", { 255,255,255,255 }, 300, troops_background);
	side_label->SetColor({ 160,25,25,255 });

	backbutton_t_b = App->gui->CreateButtonText({ 589,24 }, { 4,3 }, small_button_rect, "X", { 200,200,200,255 }, 27);
	App->gui->DisableElement(backbutton_t_b);
	troops_button = App->gui->CreateButtonText({ 16,843 }, { 32,0 }, big_button_rect, "TROOPS", { 200,200,200,255 }, 33);
	App->gui->DisableElement(troops_button);
	buildings_button = App->gui->CreateButtonText({ 319,843 }, { 10,16 }, big_button_rect, "BUILDINGS", { 200,200,200,255 }, 33);
	App->gui->DisableElement(buildings_button);


	troops_title[0] = App->gui->CreateLabel({ 30,285 }, "fonts/button_text.ttf", 33, "Infantry", { 0,0,0,0 }, 300, troops_background);
	troops_title[1] = App->gui->CreateLabel({ 400,285 }, "fonts/button_text.ttf", 33, "Land", { 0,0,0,0 }, 300, troops_background);
	troops_title[2] = App->gui->CreateLabel({ 223,605 }, "fonts/button_text.ttf", 33, "Aerial", { 0,0,0,0 }, 300, troops_background);

	if (App->game_manager->GetPlayerDeck()->cards[0])
		deck_buttons[0] = App->gui->CreateButton({ 320,30 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[0]->type, "upgrade"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[1])
		deck_buttons[1] = App->gui->CreateButton({ 450,30 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[1]->type, "upgrade"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[2])
		deck_buttons[2] = App->gui->CreateButton({ 320, 140 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[2]->type, "upgrade"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[3])
		deck_buttons[3] = App->gui->CreateButton({ 450, 140 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[3]->type, "upgrade"), troops_background);

	collection_buttons_allies[0] = App->gui->CreateButtonTroops({ 40,347 }, App->gui->LoadUIButton(2, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(2));
	collection_buttons_allies[1] = App->gui->CreateButtonTroops({ 170,347 }, App->gui->LoadUIButton(4, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(4));
	collection_buttons_allies[2] = App->gui->CreateButtonTroops({ 110,477 }, App->gui->LoadUIButton(6, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(6));
	collection_buttons_allies[3] = App->gui->CreateButtonTroops({ 350,347 }, App->gui->LoadUIButton(8, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(8));
	collection_buttons_allies[4] = App->gui->CreateButtonTroops({ 480,347 }, App->gui->LoadUIButton(10, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(10));
	collection_buttons_allies[5] = App->gui->CreateButtonTroops({ 420,477 }, App->gui->LoadUIButton(12, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(12));
	collection_buttons_allies[6] = App->gui->CreateButtonTroops({ 90,685 }, App->gui->LoadUIButton(14, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(14));
	collection_buttons_allies[7] = App->gui->CreateButtonTroops({ 260,685 }, App->gui->LoadUIButton(16, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(16));
	collection_buttons_allies[8] = App->gui->CreateButtonTroops({ 430,685 }, App->gui->LoadUIButton(18, "upgrade"), LVL_1, troops_background, App->game_manager->IsInCollection(18));

	collection_buttons_enemies[0] = App->gui->CreateButton({ 40,347 }, App->gui->LoadUIButton(1, "upgrade"), troops_background, App->game_manager->IsInCollection(1));
	collection_buttons_enemies[1] = App->gui->CreateButton({ 170,347 }, App->gui->LoadUIButton(3, "upgrade"), troops_background, App->game_manager->IsInCollection(3));
	collection_buttons_enemies[2] = App->gui->CreateButton({ 110,477 }, App->gui->LoadUIButton(5, "upgrade"), troops_background, App->game_manager->IsInCollection(5));
	collection_buttons_enemies[3] = App->gui->CreateButton({ 350,347 }, App->gui->LoadUIButton(7, "upgrade"), troops_background, App->game_manager->IsInCollection(7));
	collection_buttons_enemies[4] = App->gui->CreateButton({ 480,347 }, App->gui->LoadUIButton(9, "upgrade"), troops_background, App->game_manager->IsInCollection(9));
	collection_buttons_enemies[5] = App->gui->CreateButton({ 420,477 }, App->gui->LoadUIButton(11, "upgrade"), troops_background, App->game_manager->IsInCollection(11));
	collection_buttons_enemies[6] = App->gui->CreateButton({ 90,685 }, App->gui->LoadUIButton(13, "upgrade"), troops_background, App->game_manager->IsInCollection(13));
	collection_buttons_enemies[7] = App->gui->CreateButton({ 260,685 }, App->gui->LoadUIButton(15, "upgrade"), troops_background, App->game_manager->IsInCollection(15));
	collection_buttons_enemies[8] = App->gui->CreateButton({ 430,685 }, App->gui->LoadUIButton(17, "upgrade"), troops_background, App->game_manager->IsInCollection(17));

	App->gui->DisableElement(troops_background);

	//Building Menu

	buildings_background = App->gui->CreateImage({ 8, 20 }, { 3336,6,662,912 }, main_panel);
	building_title = App->gui->CreateLabel({ 260, 360 }, "fonts/button_text.ttf", 22, "Infantry Building:", { 242, 222, 70, 255 }, 600, buildings_background);

	buildings_title[0] = App->gui->CreateLabel({ 30,635 }, "fonts/button_text.ttf", 20, "Infantry", { 0,0,0,0 }, 300, buildings_background);
	buildings_title[1] = App->gui->CreateLabel({ 270,635 }, "fonts/button_text.ttf", 20, "Land", { 0,0,0,0 }, 300, buildings_background);
	buildings_title[2] = App->gui->CreateLabel({ 460,635 }, "fonts/button_text.ttf", 20, "Aerial", { 0,0,0,0 }, 300, buildings_background);

	//Infantry
	str = "The infantry building is the place where the soldiers rest and prepare for battle.\n\nConquered: " + std::to_string(((LeveledUpgrade*)App->game_manager->infantry_upgrade)->GetLevel())
		+ "\nDamage increase: " + std::to_string(((LeveledUpgrade*)App->game_manager->infantry_upgrade)->GetBuffValue("damage")) + "\nHealth increase: " + std::to_string(((LeveledUpgrade*)App->game_manager->infantry_upgrade)->GetBuffValue("health"));

	building_infantry_button = App->gui->CreateSelectableButton({ 17, 665 }, App->gui->LoadUIButton(30, "button"), buildings_background);
	building_infantry_image = App->gui->CreateImage({ 26,395 }, App->gui->LoadUIImage(30, "building"), buildings_background);
	building_infantry_info = App->gui->CreateLabel({ 235,-5 }, "fonts/button_text.ttf", 15, str, { 231,216,145,255 }, 350, building_infantry_image);

	//Aerial
	str = "The aerial building is where the helicopters and planes are parked.\n\nConquered: " + std::to_string(((LeveledUpgrade*)App->game_manager->aerial_upgrade)->GetLevel())
		+ "\nDamage increase: " + std::to_string(((LeveledUpgrade*)App->game_manager->aerial_upgrade)->GetBuffValue("damage")) + "\nHealth increase: " + std::to_string(((LeveledUpgrade*)App->game_manager->aerial_upgrade)->GetBuffValue("health"));
	building_aerial_button = App->gui->CreateSelectableButton({ 401,685 }, App->gui->LoadUIButton(31, "button"), buildings_background);
	building_aerial_image = App->gui->CreateImage({ 30,425 }, App->gui->LoadUIImage(31, "building"), buildings_background);
	building_aerial_info = App->gui->CreateLabel({ 230, -35 }, "fonts/button_text.ttf", 15, str, { 231,216,145,255 }, 300, building_aerial_image);

	//Land
	str = "The land building is where the tanks are waiting for the battle.\n\nConquered: " + std::to_string(((LeveledUpgrade*)App->game_manager->land_upgrade)->GetLevel())
		+ "\nDamage increase: " + std::to_string(((LeveledUpgrade*)App->game_manager->land_upgrade)->GetBuffValue("damage")) + "\nHealth increase: " + std::to_string(((LeveledUpgrade*)App->game_manager->land_upgrade)->GetBuffValue("health"));
	building_land_button = App->gui->CreateSelectableButton({ 210,652 } , App->gui->LoadUIButton(32, "button"), buildings_background);
	building_land_image = App->gui->CreateImage({ 26,395 } , App->gui->LoadUIImage(32, "building"), buildings_background);
	building_land_info = App->gui->CreateLabel({ 235,-5 } , "fonts/button_text.ttf", 15, str, { 231,216,145,255 }, 300, building_land_image);

	//Core
	core_image = App->gui->CreateImage({ 40, 130 }, { 1538,23,173,114 }, buildings_background);

	core_title = App->gui->CreateLabel({ 20, 15 }, "fonts/button_text.ttf", 43, "CORE", { 242, 222, 70, 255 }, 200, buildings_background);
	core_info = App->gui->CreateLabel({ 260, 72 }, "fonts/button_text.ttf", 15, "The core travels around the map destroying all the enemies bases.", { 231,216,145,255 }, 300, buildings_background);

	str = "Health: " + std::to_string((int)App->game_manager->stats.find("health")->second->GetValue());
	core_health = App->gui->CreateLabel({ 260, 150 }, "fonts/button_text.ttf", 16, str, { 231,216,145,255 }, 200, buildings_background);

	str = "Energy: " + std::to_string((int)App->game_manager->stats.find("energy")->second->GetValue());
	core_energy = App->gui->CreateLabel({ 260, 170 }, "fonts/button_text.ttf", 16, str, { 231,216,145,255 }, 200, buildings_background);

	uint cost = ((LeveledUpgrade*)App->game_manager->health_upgrade)->GetCost();
	str = cost > 0 ? "Cost Health: " + std::to_string(cost) : "Maxed";
	core_lvl_up_health_cost = App->gui->CreateLabel({ 272,210 }, "fonts/button_text.ttf", 10, str, { 231,216,145,255 }, 200, buildings_background);
	core_lvl_up_health = App->gui->CreateButtonText({ 270, 230 }, { 10,5 }, little_button_rect, "HEALTH UP", { 242, 222, 70, 255 }, 14, buildings_background);
	if (cost == 0)
		core_lvl_up_health->SetLocked(false);

	cost = ((LeveledUpgrade*)App->game_manager->energy_upgrade)->GetCost();
	str = cost > 0 ? "Cost Energy: " + std::to_string(cost) : "Maxed";
	core_lvl_up_energy_cost = App->gui->CreateLabel({ 442,210 }, "fonts/button_text.ttf", 10, str, { 231,216,145,255 }, 200, buildings_background);
	core_lvl_up_energy = App->gui->CreateButtonText({ 440, 230 }, { 10,5 }, little_button_rect, "ENERGY UP", { 242, 222, 70, 255 }, 14, buildings_background);
	if (cost == 0)
		core_lvl_up_energy->SetLocked(false);


	App->gui->DisableElement(buildings_background);

	//Show Info
	info_image = App->gui->CreateImage({ 25,37 }, { 0,0,0,0 }, troops_background);
	health_label = App->gui->CreateLabel({ 135, 40 }, "fonts/button_text.ttf", 12, "Health: -", { 231,216,145,255 }, 120, troops_background);
	attack_label = App->gui->CreateLabel({ 135, 60 }, "fonts/button_text.ttf", 12, "Attack: -", { 231,216,145,255 }, 120, troops_background);
	defense_label = App->gui->CreateLabel({ 135, 80 }, "fonts/button_text.ttf", 12, "Defense: -", { 231,216,145,255 }, 120, troops_background);
	range_label = App->gui->CreateLabel({ 135, 100 }, "fonts/button_text.ttf", 12, "Range: -", { 231,216,145,255 }, 120, troops_background);
	units_label = App->gui->CreateLabel({ 135, 120 }, "fonts/button_text.ttf", 12, "Units: -", { 231,216,145,255 }, 120, troops_background);

	energy_bar = App->gui->CreateBar({ 8,188 }, { 2897,1780,260,65 }, App->game_manager->GetCardFromCollection(CONSCRIPT)->info.stats.find("energy_cost")->second, BAR_HORITZONTAL, BAR_STATIC, nullptr, troops_background);

	//Gold
	str = "Gold: " + std::to_string(App->game_manager->gold) + "g";
	gold_quantity = App->gui->CreateLabel({ 460, 150 }, "fonts/button_text.ttf", 16, str, { 231,216,145,255 }, 200, buildings_background);
}

bool StrategyMapScene::IsInsideLimits(int mousemotion_x, int mousemotion_y)
{

	int camera_x = -App->render->camera.x;
	int camera_y = -App->render->camera.y;

	if (camera_x + App->render->camera.w - mousemotion_x < map_camera_limit.x + map_camera_limit.w && camera_y + App->render->camera.h - mousemotion_y < map_camera_limit.y + map_camera_limit.h
		&& camera_x - mousemotion_x > map_camera_limit.x && camera_y - mousemotion_y > map_camera_limit.y) return true;
	else return false;
}

void StrategyMapScene::KeepInBounds()
{
	int camera_x = -App->render->camera.x;
	int camera_y = -App->render->camera.y;

	if (camera_x + App->render->camera.w > map_camera_limit.x + map_camera_limit.w)
		App->render->camera.x = -((map_camera_limit.x + map_camera_limit.w) - App->render->camera.w);

	if (camera_y + App->render->camera.h > map_camera_limit.y + map_camera_limit.h)
		App->render->camera.y = -((map_camera_limit.y + map_camera_limit.h) - App->render->camera.h);

	if (camera_x < map_camera_limit.x)
		App->render->camera.x = -map_camera_limit.x;

	if (camera_y < map_camera_limit.y)
		App->render->camera.y = -map_camera_limit.y;
}
