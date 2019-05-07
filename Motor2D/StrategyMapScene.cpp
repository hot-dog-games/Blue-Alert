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
	App->game_manager->Restart();

	BROFILER_CATEGORY("SMStart", Profiler::Color::Red);

	App->map->Load("Nodes Map.tmx");
	App->ResumeGame();

	App->game_manager->GetEncounterTree()->CreateAllNodes();
	App->game_manager->GetEncounterTree()->UpdateTreeState();

	uint w, h;
	App->win->GetWindowSize(w, h);

	
	iPoint world_position = App->map->MapToWorld((int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().x, (int)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetPosition().y);

	App->render->camera.x = -world_position.x + w * 0.5;
	App->render->camera.y = -world_position.y + h * 0.9;

	StartUI();
	
	App->audio->PlayMusic("audio/music/5.InDeep-RedAlert2_2.ogg");


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
			//(options = App->gui->CreateImage();
		}
		else if (element == menu_button) {
			

			App->gui->EnableElement(troops_button);
			App->gui->EnableElement(buildings_button);
			App->gui->EnableElement(troops_background);
			App->gui->EnableElement(backbutton_t_b);

			App->gui->DisableElement(menu_button);
			App->gui->DisableElement(settings_button);

			App->game_manager->GetEncounterTree()->is_clickable = false;

			for (uint i = 0; i < 4; ++i) {
				if(!App->game_manager->GetPlayerDeck()->cards[i])
					App->gui->DisableElement(deck_buttons[i]);
			}

		}
		else if (element == backbutton_t_b) {
			App->gui->DisableElement(troops_background);
			App->gui->DisableElement(backbutton_t_b);
			App->gui->DisableElement(troops_button);
			App->gui->DisableElement(buildings_button);

			App->gui->EnableElement(menu_button);
			App->gui->EnableElement(settings_button);

			App->game_manager->GetEncounterTree()->is_clickable = true;


		}
		else if (element == deck_buttons[0])
		{
			App->gui->DisableElement(deck_buttons[0]);
			App->game_manager->GetPlayerDeck()->RemoveCard(0);
		}
		else if (element == deck_buttons[1])
		{
			App->gui->DisableElement(deck_buttons[1]);
			App->game_manager->GetPlayerDeck()->RemoveCard(1);
		}
		else if (element == deck_buttons[2])
		{
			App->gui->DisableElement(deck_buttons[2]);
			App->game_manager->GetPlayerDeck()->RemoveCard(2);
		}
		else if (element == deck_buttons[3])
		{
			App->gui->DisableElement(deck_buttons[3]);
			App->game_manager->GetPlayerDeck()->RemoveCard(3);
		}
		else if (element == collection_buttons_allies[0])
		{
			AddCardToDeck(element,0);
		}
		else if (element == collection_buttons_allies[1])
		{
			AddCardToDeck(element,2);
		}
		else if (element == collection_buttons_allies[2])
		{
			AddCardToDeck(element,4);
		}
		else if (element == collection_buttons_allies[3])
		{
			AddCardToDeck(element,6);
		}
		else if (element == collection_buttons_allies[4])
		{
			AddCardToDeck(element,8);
		}
		else if (element == collection_buttons_allies[5])
		{
			AddCardToDeck(element,10);
		}
		else if (element == collection_buttons_allies[6])
		{
			AddCardToDeck(element,12);
		}
		else if (element == collection_buttons_allies[7])
		{
			AddCardToDeck(element,14);
		}
		else if (element == collection_buttons_allies[8])
		{
			AddCardToDeck(element,16);
		}
		else if (element == collection_buttons_allies[9])
		{
			AddCardToDeck(element,9);
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
	}
}

void StrategyMapScene::StartUI()
{
	uint w, h;
	App->win->GetWindowSize(w, h);

	main_panel = App->gui->CreateImage({ 0,0 }, { 0, 0, 0, 0}, nullptr, false);
	banner = App->gui->CreateImage({ 4,5 }, { 1,769,1017,83 }, main_panel);

	SDL_Rect small_button_rect[3];
	small_button_rect[0] = { 753, 497, 41, 40 };
	small_button_rect[1] = { 753, 538, 41, 40 };
	small_button_rect[2] = { 753, 577, 41, 40 };

	SDL_Rect medium_button_rect[3];
	medium_button_rect[0] = { 800,499,294,67 };
	medium_button_rect[1] = { 800,569,294,67 };
	medium_button_rect[2] = { 800,639,294,67 };

	SDL_Rect large_button_rect[3];
	large_button_rect[0] = { 449, 498, 304, 74 };
	large_button_rect[1] = { 449, 577, 304, 74 };
	large_button_rect[2] = { 449, 653, 304, 74 };

	settings_button = App->gui->CreateButton({ 50,700 }, small_button_rect, main_panel);
	menu_button = App->gui->CreateButton({ 700,700 }, large_button_rect, main_panel);
	text_menu = App->gui->CreateLabel({ 50,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "MENU", { 0,0,0,0 }, 0, menu_button);

	gold = App->gui->CreateLabel({ 90, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "GOLD", { 0,0,0,0 }, 0, main_panel);
	energy = App->gui->CreateLabel({ 450, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "ENERGY", { 0,0,0,0 }, 0, main_panel);
	health = App->gui->CreateLabel({ 860, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "HEALTH", { 0,0,0,0 }, 0, main_panel);


	// Troops menu
	troops_background = App->gui->CreateImage({ 20,95 }, { 746,1218,990,600 }, main_panel);

	backbutton_t_b = App->gui->CreateButton({ 930,10 }, small_button_rect, troops_background);
	back_cross_text = App->gui->CreateLabel({ 15,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 28, "X", { 0,0,0,0 }, 0, backbutton_t_b);
	App->gui->DisableElement(backbutton_t_b);
	troops_button = App->gui->CreateButton({ 380,10 }, large_button_rect, troops_background);
	troops_text = App->gui->CreateLabel({ 50,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "TROOPS", { 0,0,0,0 }, 0, troops_button);
	App->gui->DisableElement(troops_button);
	buildings_button = App->gui->CreateButton({ 600,10 }, large_button_rect, troops_background);
	buildings_text = App->gui->CreateLabel({ 35,10 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "BUILDINGS", { 0,0,0,0 }, 0, buildings_button);
	App->gui->DisableElement(buildings_button);


	troops_title[0] = App->gui->CreateLabel({ 30,285 }, "fonts/button_text.ttf", 33, "Infantry", { 0,0,0,0 }, 300, troops_background);
	troops_title[1] = App->gui->CreateLabel({ 420,285 }, "fonts/button_text.ttf", 33, "Land", { 0,0,0,0 }, 300, troops_background);
	troops_title[2] = App->gui->CreateLabel({ 733,285 }, "fonts/button_text.ttf", 33, "Aerial", { 0,0,0,0 }, 300, troops_background);

	if(App->game_manager->GetPlayerDeck()->cards[0])
		deck_buttons[0] = App->gui->CreateButton({ 360,99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[0]->type, "deck"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[1])
		deck_buttons[1] = App->gui->CreateButton({ 500,99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[1]->type, "deck"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[2])
		deck_buttons[2] = App->gui->CreateButton({ 640,99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[2]->type, "deck"), troops_background);
	if (App->game_manager->GetPlayerDeck()->cards[3])
		deck_buttons[3] = App->gui->CreateButton({ 780,99 }, App->gui->LoadUIButton(App->game_manager->GetPlayerDeck()->cards[3]->type, "deck"), troops_background);

	collection_buttons_allies[0] = App->gui->CreateButton({ 40,347 }, App->gui->LoadUIButton(2, "upgrade"), troops_background);
	collection_buttons_allies[1] = App->gui->CreateButton({ 170,347 }, App->gui->LoadUIButton(4, "upgrade"), troops_background);
	collection_buttons_allies[2] = App->gui->CreateButton({ 110,477 }, App->gui->LoadUIButton(6, "upgrade"), troops_background);
	collection_buttons_allies[3] = App->gui->CreateButton({ 380,347 }, App->gui->LoadUIButton(8, "upgrade"), troops_background);
	collection_buttons_allies[4] = App->gui->CreateButton({ 510,347 }, App->gui->LoadUIButton(10, "upgrade"), troops_background);
	collection_buttons_allies[5] = App->gui->CreateButton({ 445,477 }, App->gui->LoadUIButton(12, "upgrade"), troops_background);
	collection_buttons_allies[6] = App->gui->CreateButton({ 720,347 }, App->gui->LoadUIButton(14, "upgrade"), troops_background);
	collection_buttons_allies[7] = App->gui->CreateButton({ 840,347 }, App->gui->LoadUIButton(16, "upgrade"), troops_background);
	collection_buttons_allies[8] = App->gui->CreateButton({ 780,477 }, App->gui->LoadUIButton(18, "upgrade"), troops_background);

	App->gui->DisableElement(troops_background);

	//Building Menu

	buildings_background = App->gui->CreateImage({ 20,95 }, { 1780,1229,986,593 }, main_panel);
	building_title = App->gui->CreateLabel({ 610, 70 }, "fonts/button_text.ttf", 22, "Aerial Building:", { 242, 222, 70, 255 }, 600, buildings_background);

	building_infantry_button = App->gui->CreateSelectableButton({343, 400 }, App->gui->LoadUIButton(30, "button"),buildings_background);
	building_infantry_image = App->gui->CreateImage({ 365,125 }, App->gui->LoadUIImage(30, "building"), buildings_background);
	building_infantry_info = App->gui->CreateLabel({ 245,-30 }, "fonts/red_alert.ttf", 20, "The infantry troops are upgraded by %i", { 231,216,145,255 }, 300, building_infantry_image);
	
	building_aerial_button = App->gui->CreateSelectableButton({ 770,400 }, App->gui->LoadUIButton(31, "button"), buildings_background);
	building_aerial_image = App->gui->CreateImage({ 345,115 }, App->gui->LoadUIImage(31, "building"), buildings_background);
	building_aerial_info = App->gui->CreateLabel({ 265,-20 }, "fonts/red_alert.ttf", 20, "The aerial troops are upgraded by %i", { 231,216,145,255 }, 300, building_aerial_image);
	
	building_land_button = App->gui->CreateSelectableButton({ 560,430 } , App->gui->LoadUIButton(32, "button"), buildings_background);
	building_land_image = App->gui->CreateImage({ 350,145 }, App->gui->LoadUIImage(32, "building"), buildings_background);
	building_land_info = App->gui->CreateLabel({ 260, -50 }, "fonts/red_alert.ttf", 20, "The land troops are upgraded by %i", { 231,216,145,255 }, 300, building_land_image);

	App->gui->DisableElement(buildings_background);
}
