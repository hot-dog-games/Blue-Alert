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
#include "Brofiler/Brofiler.h"
#include "Stat.h"
#include "UIBar.h"
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
	App->game_manager->Restart();

	BROFILER_CATEGORY("SMStart", Profiler::Color::Red);

	switch (App->game_manager->stage)
	{
	case STAGE_TUTORIAL: App->map->Load("Tutorial_Nodes_Map.tmx"); break;
	case STAGE_01: App->map->Load("Nodes Map.tmx"); break;
	default:
		break;
	}
	App->ResumeGame();

	App->game_manager->GetEncounterTree()->CreateAllNodes();
	App->game_manager->GetEncounterTree()->UpdateTreeState();

	uint w, h;
	App->win->GetWindowSize(w, h);

	iPoint world_position = App->map->MapToWorld((int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().x, (int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().y);

	App->render->camera.x = -world_position.x + w * 0.5;
	App->render->camera.y = -world_position.y + h * 0.9;

	if (!IsInsideLimits(0, 0))
	{
		App->render->camera.x = -limit_center.x;
		App->render->camera.y = -limit_center.y;
	}

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
			if (IsInsideLimits(mousemotion_x, mousemotion_y))
			{
				if (abs(mousemotion_x) > drag_threshhold && abs(mousemotion_y) > drag_threshhold)
				{
					App->render->camera.x += mousemotion_x;
					App->render->camera.y += mousemotion_y;
				}
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

	last_camera_position.x = App->render->camera.x;
	last_camera_position.y = App->render->camera.y;

	return true;
}

// Called each loop iteration
bool StrategyMapScene::Update(float dt)
{	
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

			App->gui->DisableElement(menu_button);
			App->gui->DisableElement(settings_button);

			App->game_manager->GetEncounterTree()->is_clickable = false;
			dragable = false;

		}
		else if (element == resume_settings_button) {
			App->gui->DisableElement(options);

			App->gui->EnableElement(menu_button);
			App->gui->EnableElement(settings_button);
			App->game_manager->GetEncounterTree()->is_clickable = true;
			dragable = true;
		}

		else if (element == menu_button || (element == troops_button && !troops_background->IsEnabled())) {
			
			App->gui->EnableElement(troops_button);
			App->gui->EnableElement(buildings_button);
			App->gui->EnableElement(troops_background);
			App->gui->EnableElement(backbutton_t_b);

			for (int i = 0; i < 9; ++i) {
				App->gui->DisableElement(collection_buttons_enemies[i]);
			}

			App->gui->DisableElement(menu_button);
			App->gui->DisableElement(settings_button);
			App->gui->DisableElement(buildings_background);

			App->game_manager->GetEncounterTree()->is_clickable = false;
			dragable = false;

			for (uint i = 0; i < 4; ++i) {
				if(!App->game_manager->GetPlayerDeck()->cards[i])
					App->gui->DisableElement(deck_buttons[i]);
			}

			building_aerial_button->ChangeState(true);
			building_title->SetText("Aerial Building:");

		}
		else if (element == backbutton_t_b) {
			App->gui->DisableElement(troops_background);
			App->gui->DisableElement(buildings_background);
			App->gui->DisableElement(backbutton_t_b);
			App->gui->DisableElement(troops_button);
			App->gui->DisableElement(buildings_button);

			App->gui->EnableElement(menu_button);
			App->gui->EnableElement(settings_button);

			App->game_manager->GetEncounterTree()->is_clickable = true;
			dragable = true;


		}
		else if (element == buildings_button) {
			App->gui->DisableElement(troops_background);
			App->gui->EnableElement(buildings_background);
			App->gui->DisableElement(building_infantry_image);
			App->gui->DisableElement(building_land_image);
			building_aerial_button->ChangeState(true);
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

		if (element == core_lvl_up_health && ((int)App->game_manager->gold >= ((LeveledUpgrade*)App->game_manager->health_upgrade)->GetCost())) {
			App->game_manager->gold -= ((LeveledUpgrade*)App->game_manager->health_upgrade)->GetCost();
			App->game_manager->UpgradeHealth();

			std::string str;

			str = "Health: " + std::to_string((int)App->game_manager->stats.find("health")->second->GetValue());
			core_health->SetText(str);

			str = "Cost Health: " + std::to_string(((LeveledUpgrade*)App->game_manager->health_upgrade)->GetCost());
			core_lvl_up_health_cost->SetText(str);

			str = "GOLD: " + std::to_string(App->game_manager->gold);
			gold->SetText(str);
		}
		else if (element == core_lvl_up_energy && ((int)App->game_manager->gold >= ((LeveledUpgrade*)App->game_manager->energy_upgrade)->GetCost())) {
			App->game_manager->gold -= ((LeveledUpgrade*)App->game_manager->energy_upgrade)->GetCost();
			App->game_manager->UpgradeEnergy();

			std::string str;
			str = "Energy: " + std::to_string((int)App->game_manager->stats.find("energy")->second->GetValue());
			core_energy->SetText(str);

			str = "Cost Energy: " + std::to_string(((LeveledUpgrade*)App->game_manager->energy_upgrade)->GetCost());
			core_lvl_up_energy_cost->SetText(str);

			str = "GOLD: " + std::to_string(App->game_manager->gold);
			gold->SetText(str);
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
			}
			else {
				App->gui->DisableElement(building_infantry_image);
				building_title->SetText("");
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
			}
			else {
				App->gui->DisableElement(building_land_image);
				building_title->SetText("");
			}
		}
	}
	else if (gui_event == GUI_Event::MOUSE_OVER)
	{
	std::string str_stat;
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
			deck_buttons[i]->ChangeSprite(App->gui->LoadUIButton((App->game_manager->GetPlayerDeck()->cards[i]->type), "deck"));
			break;
		}
		else if (!deck_buttons[i] && card && !App->game_manager->IsInPlayerDeck(card)) {
			App->game_manager->GetPlayerDeck()->AddCard(card);
			deck_buttons[i] = App->gui->CreateButton({ (int)(360 + i * 140), 99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[i]->type, "deck"), troops_background);
			App->gui->EnableElement(deck_buttons[i]);
			deck_buttons[i]->ChangeSprite(App->gui->LoadUIButton((App->game_manager->GetPlayerDeck()->cards[i]->type), "deck"));
			break;
		}
	}
}

void StrategyMapScene::InitializeUI()
{
	uint w, h;
	App->win->GetWindowSize(w, h);

	main_panel = App->gui->CreateImage({ 0,0 }, { 0, 0, 0, 0 }, nullptr, false);
	banner = App->gui->CreateImage({ 4,5 }, { 1,769,1017,83 }, main_panel);

	SDL_Rect small_button_rect[3];
	small_button_rect[0] = { 753, 497, 41, 40 };
	small_button_rect[1] = { 753, 538, 41, 40 };
	small_button_rect[2] = { 753, 577, 41, 40 };

	SDL_Rect medium_button_rect[4];
	medium_button_rect[0] = { 800,499,294,67 };
	medium_button_rect[1] = { 800,569,294,67 };
	medium_button_rect[2] = { 800,639,294,67 };
	medium_button_rect[3] = { 800,639,294,67 };

	SDL_Rect little_button_rect[3];
	little_button_rect[0] = { 1256,379,145,67 };
	little_button_rect[1] = { 1256,449,145,67 };
	little_button_rect[2] = { 1256,519,145,67 };

	SDL_Rect change_button[3];
	change_button[0] = { 3,700,53,51 };
	change_button[1] = { 77,700,53,51 };
	change_button[2] = { 148,700,53,51 };

	SDL_Rect options_rect[3];
	options_rect[0] = {4530,1200,77,60};
	options_rect[1] = {4528,1267,82,66};
	options_rect[2] = {4530,1339,77,61};

	std::string str;

	menu_button = App->gui->CreateButtonText({ 700,700 }, { 70,0 }, medium_button_rect, "MENU", { 200,200,200,255 }, 33, main_panel);
	options = App->gui->CreateImage({ 200,100 }, { 0,2444,619,463 }, nullptr);
	settings_button = App->gui->CreateButton({ 50,700 }, options_rect, main_panel);
	resume_settings_button = App->gui->CreateButton({ 320,380 }, medium_button_rect, options);
	resume_label = App->gui->CreateLabel({ 60,15 }, "fonts/button_text.ttf", 20, "Resume", { 255,255,255,0 }, 0, resume_settings_button);

	music_slider = App->gui->CreateScrollBar({ 350,150 }, { 4506,1062,218,40 }, MUSIC, volume, 128, options);
	musiclabel = App->gui->CreateLabel({ 50,160 }, "fonts/button_text.ttf", 20, "Music Volume", { 255,255,255,0 }, 0, options);
	fx_slider = App->gui->CreateScrollBar({ 350,250 }, { 4506,1062,218,40 }, FX, volume, 128, options);
	fxlabel = App->gui->CreateLabel({ 50,260 }, "fonts/button_text.ttf", 20, "FX Volume", { 255,255,255,0 }, 0, options);
	savegamebutton = App->gui->CreateButton({ 30,380 }, medium_button_rect, options, false);
	savegamelabel = App->gui->CreateLabel({ 40,15 }, "fonts/button_text.ttf", 20, "Save Game", { 255,0,0,0 }, 0, savegamebutton);
	App->gui->DisableElement(options);

	str = "GOLD: " + std::to_string(App->game_manager->gold);
	gold = App->gui->CreateLabel({ 60, 30 }, "fonts/button_text.ttf", 25, str, { 0,0,0,0 }, 0, main_panel);

	// Troops menu
	troops_background = App->gui->CreateImage({ 20,95 }, { 3711,5,985,659 }, main_panel);
	change_side_button = App->gui->CreateButton({ 918,597 }, change_button, troops_background);
	side_label = App->gui->CreateLabel({ 700, 607 }, "fonts/button_text.ttf", 30, "Allies", { 255,255,255,255 }, 300, troops_background);
	side_label->SetColor({ 160,25,25,255 });

	backbutton_t_b = App->gui->CreateButtonText({ 961,99 }, { 4,3 }, small_button_rect, "X", { 200,200,200,255 }, 27);
	App->gui->DisableElement(backbutton_t_b);
	troops_button = App->gui->CreateButtonText({ 351,98 }, { 32,0 }, medium_button_rect, "TROOPS", { 200,200,200,255 }, 33);
	App->gui->DisableElement(troops_button);
	buildings_button = App->gui->CreateButtonText({ 635,98 }, { 10,16 }, medium_button_rect, "BUILDINGS", { 200,200,200,255 }, 33);
	App->gui->DisableElement(buildings_button);


	troops_title[0] = App->gui->CreateLabel({ 30,285 }, "fonts/button_text.ttf", 33, "Infantry", { 0,0,0,0 }, 300, troops_background);
	troops_title[1] = App->gui->CreateLabel({ 420,285 }, "fonts/button_text.ttf", 33, "Land", { 0,0,0,0 }, 300, troops_background);
	troops_title[2] = App->gui->CreateLabel({ 733,285 }, "fonts/button_text.ttf", 33, "Aerial", { 0,0,0,0 }, 300, troops_background);

	if (App->game_manager->GetPlayerDeck()->cards[0])
		deck_buttons[0] = App->gui->CreateButton({ 360,99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[0]->type, "deck"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[1])
		deck_buttons[1] = App->gui->CreateButton({ 500,99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[1]->type, "deck"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[2])
		deck_buttons[2] = App->gui->CreateButton({ 640,99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[2]->type, "deck"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[3])
		deck_buttons[3] = App->gui->CreateButton({ 780,99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[3]->type, "deck"), troops_background);

	collection_buttons_allies[0] = App->gui->CreateButton({ 40,347 }, App->gui->LoadUIButton(2, "upgrade"), troops_background, App->game_manager->IsInCollection(2));
	collection_buttons_allies[1] = App->gui->CreateButton({ 170,347 }, App->gui->LoadUIButton(4, "upgrade"), troops_background, App->game_manager->IsInCollection(4));
	collection_buttons_allies[2] = App->gui->CreateButton({ 110,477 }, App->gui->LoadUIButton(6, "upgrade"), troops_background, App->game_manager->IsInCollection(6));
	collection_buttons_allies[3] = App->gui->CreateButton({ 380,347 }, App->gui->LoadUIButton(8, "upgrade"), troops_background, App->game_manager->IsInCollection(8));
	collection_buttons_allies[4] = App->gui->CreateButton({ 510,347 }, App->gui->LoadUIButton(10, "upgrade"), troops_background, App->game_manager->IsInCollection(10));
	collection_buttons_allies[5] = App->gui->CreateButton({ 445,477 }, App->gui->LoadUIButton(12, "upgrade"), troops_background, App->game_manager->IsInCollection(12));
	collection_buttons_allies[6] = App->gui->CreateButton({ 720,347 }, App->gui->LoadUIButton(14, "upgrade"), troops_background, App->game_manager->IsInCollection(14));
	collection_buttons_allies[7] = App->gui->CreateButton({ 840,347 }, App->gui->LoadUIButton(16, "upgrade"), troops_background, App->game_manager->IsInCollection(16));
	collection_buttons_allies[8] = App->gui->CreateButton({ 780,477 }, App->gui->LoadUIButton(18, "upgrade"), troops_background, App->game_manager->IsInCollection(18));

	collection_buttons_enemies[0] = App->gui->CreateButton({ 40,347 }, App->gui->LoadUIButton(1, "upgrade"), troops_background, App->game_manager->IsInCollection(1));
	collection_buttons_enemies[1] = App->gui->CreateButton({ 170,347 }, App->gui->LoadUIButton(3, "upgrade"), troops_background, App->game_manager->IsInCollection(3));
	collection_buttons_enemies[2] = App->gui->CreateButton({ 110,477 }, App->gui->LoadUIButton(5, "upgrade"), troops_background, App->game_manager->IsInCollection(5));
	collection_buttons_enemies[3] = App->gui->CreateButton({ 380,347 }, App->gui->LoadUIButton(7, "upgrade"), troops_background, App->game_manager->IsInCollection(7));
	collection_buttons_enemies[4] = App->gui->CreateButton({ 510,347 }, App->gui->LoadUIButton(9, "upgrade"), troops_background, App->game_manager->IsInCollection(9));
	collection_buttons_enemies[5] = App->gui->CreateButton({ 445,477 }, App->gui->LoadUIButton(11, "upgrade"), troops_background, App->game_manager->IsInCollection(11));
	collection_buttons_enemies[6] = App->gui->CreateButton({ 720,347 }, App->gui->LoadUIButton(13, "upgrade"), troops_background, App->game_manager->IsInCollection(13));
	collection_buttons_enemies[7] = App->gui->CreateButton({ 840,347 }, App->gui->LoadUIButton(15, "upgrade"), troops_background, App->game_manager->IsInCollection(15));
	collection_buttons_enemies[8] = App->gui->CreateButton({ 780,477 }, App->gui->LoadUIButton(17, "upgrade"), troops_background, App->game_manager->IsInCollection(17));

	App->gui->DisableElement(troops_background);

	//Building Menu

	buildings_background = App->gui->CreateImage({ 20,95 }, { 1780,1229,986,593 }, main_panel);
	building_title = App->gui->CreateLabel({ 610, 70 }, "fonts/button_text.ttf", 22, "Aerial Building:", { 242, 222, 70, 255 }, 600, buildings_background);

	buildings_title[0] = App->gui->CreateLabel({ 343,385 }, "fonts/button_text.ttf", 22, "Infantry", { 0,0,0,0 }, 300, buildings_background);
	buildings_title[1] = App->gui->CreateLabel({ 605,385 }, "fonts/button_text.ttf", 22, "Land", { 0,0,0,0 }, 300, buildings_background);
	buildings_title[2] = App->gui->CreateLabel({ 812,385 }, "fonts/button_text.ttf", 22, "Aerial", { 0,0,0,0 }, 300, buildings_background);

	//Infantry
	str = "The infantry building is the place where the soldiers rest and prepare for battle.\n\nConquered: " + std::to_string(App->game_manager->GetEncounterTree()->GetBuildingsOfType(EntityType::INFANTRY_STRATEGY_BUILDING));

	building_infantry_button = App->gui->CreateSelectableButton({ 355, 435 }, App->gui->LoadUIButton(30, "button"), buildings_background);
	building_infantry_image = App->gui->CreateImage({ 365,125 }, App->gui->LoadUIImage(30, "building"), buildings_background);
	building_infantry_info = App->gui->CreateLabel({ 245,-30 }, "fonts/red_alert.ttf", 23, str, { 231,216,145,255 }, 300, building_infantry_image);

	//Aerial
	str = "The aerial building is where the helicopters and planes are parked.\n\nConquered: " + std::to_string(App->game_manager->GetEncounterTree()->GetBuildingsOfType(EntityType::AERIAL_STRATEGY_BUILDING));
	building_aerial_button = App->gui->CreateSelectableButton({ 783,445 }, App->gui->LoadUIButton(31, "button"), buildings_background);
	building_aerial_image = App->gui->CreateImage({ 350,145 }, App->gui->LoadUIImage(31, "building"), buildings_background);
	building_aerial_info = App->gui->CreateLabel({ 260, -50 }, "fonts/red_alert.ttf", 23, str, { 231,216,145,255 }, 300, building_aerial_image);

	//Land
	str = "The land building is where the tanks are waiting for the battle.\n\nConquered: " + std::to_string(App->game_manager->GetEncounterTree()->GetBuildingsOfType(EntityType::LAND_STRATEGY_BUILDING));
	building_land_button = App->gui->CreateSelectableButton({ 560,430 } , App->gui->LoadUIButton(32, "button"), buildings_background);
	building_land_image = App->gui->CreateImage({ 345,115 } , App->gui->LoadUIImage(32, "building"), buildings_background);
	building_land_info = App->gui->CreateLabel({ 265,-20 } , "fonts/red_alert.ttf", 23, str, { 231,216,145,255 }, 300, building_land_image);

	//Core
	core_image = App->gui->CreateImage({ 70, 80 }, { 1538,23,173,114 }, buildings_background);

	core_title = App->gui->CreateLabel({ 27, 290 }, "fonts/button_text.ttf", 30, "CORE", { 242, 222, 70, 255 }, 200, buildings_background);
	core_info = App->gui->CreateLabel({ 27, 330 }, "fonts/red_alert.ttf", 23, "The core travels around the map destroying all the enemies bases.", { 231,216,145,255 }, 250, buildings_background);

	str = "Health: " + std::to_string((int)App->game_manager->stats.find("health")->second->GetValue());
	core_health = App->gui->CreateLabel({ 27, 410 }, "fonts/red_alert.ttf", 30, str, { 231,216,145,255 }, 300, buildings_background);

	str = "Energy: " + std::to_string((int)App->game_manager->stats.find("energy")->second->GetValue());
	core_energy = App->gui->CreateLabel({ 27,440 }, "fonts/red_alert.ttf", 30, str, { 231,216,145,255 }, 300, buildings_background);

	str = "Cost Health: " + std::to_string(((LeveledUpgrade*)App->game_manager->health_upgrade)->GetCost());
	core_lvl_up_health_cost = App->gui->CreateLabel({ 20,500 }, "fonts/red_alert.ttf", 21, str, { 231,216,145,255 }, 300, buildings_background);

	str = "Cost Energy: " + std::to_string(((LeveledUpgrade*)App->game_manager->energy_upgrade)->GetCost());
	core_lvl_up_energy_cost = App->gui->CreateLabel({ 159,500 }, "fonts/red_alert.ttf", 21, str, { 231,216,145,255 }, 300, buildings_background);

	core_lvl_up_health = App->gui->CreateButtonText({ 10, 520 }, { 10,5 }, little_button_rect, "HEALTH UP", { 242, 222, 70, 255 }, 14, buildings_background);
	core_lvl_up_energy = App->gui->CreateButtonText({ 154, 520 }, { 10,5 }, little_button_rect, "ENERGY UP", { 242, 222, 70, 255 }, 14, buildings_background);

	App->gui->DisableElement(buildings_background);

	//Show Info
	info_image = App->gui->CreateImage({ 25,30 }, { 0,0,0,0 }, troops_background);
	health_label = App->gui->CreateLabel({ 135, 30 }, "fonts/red_alert.ttf", 25, "Health: -", { 231,216,145,255 }, 120, troops_background);
	attack_label = App->gui->CreateLabel({ 135,55 }, "fonts/red_alert.ttf", 25, "Attack: -", { 231,216,145,255 }, 120, troops_background);
	defense_label = App->gui->CreateLabel({ 135,80 }, "fonts/red_alert.ttf", 25, "Defense: -", { 231,216,145,255 }, 120, troops_background);
	range_label = App->gui->CreateLabel({ 135, 105 }, "fonts/red_alert.ttf", 25, "Range: -", { 231,216,145,255 }, 120, troops_background);
	units_label = App->gui->CreateLabel({ 135, 130 }, "fonts/red_alert.ttf", 25, "Units: -", { 231,216,145,255 }, 120, troops_background);

	energy_bar = App->gui->CreateBar({ 8,188 }, { 2897,1780,267,64 }, App->game_manager->GetCardFromCollection(CONSCRIPT)->info.stats.find("energy_cost")->second, BAR_HORITZONTAL, BAR_STATIC, nullptr, troops_background);
}

bool StrategyMapScene::IsInsideLimits(int mousemotion_x, int mousemotion_y)
{
	float distance_center_camera = limit_center.DistanceTo({ -App->render->camera.x - mousemotion_x, -App->render->camera.y - mousemotion_y});

	if (distance_center_camera < limit_radius)return true;
	else return false;
}
