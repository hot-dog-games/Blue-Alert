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
#include "Entity.h"
#include "Core.h"
#include "CoreAI.h"
#include "PathFinding.h"
#include "UIAnimatedImage.h"
#include "UIButton.h"
#include "UIBar.h"
#include "GUI.h"
#include "Pathfinding.h"
#include "SceneManager.h"
#include "TransitionManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "TestingScene.h"
#include "BattleScene.h"
#include "GameManager.h"
#include "EncounterTree.h"
#include "EncounterNode.h"
#include "UIImage.h"
#include "UILabel.h"
#include "GameManager.h"
#include "Stat.h"
#include <stdlib.h>
#include <time.h>


TestingScene::TestingScene() : Scene()
{

}

// Destructor
TestingScene::~TestingScene()
{}

// Called before the first frame
bool TestingScene::Start()
{
	if (App->map->Load("test_grande.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path2.png");

	App->render->camera.x = (App->map->data.width*App->map->data.tile_width*0.5)*0.5 - 250;
	App->render->camera.y = -210;

	Deck* enemy_deck = new Deck();
	enemy_deck->delete_cards = true;
	enemy_deck->AddCard(App->card_manager->CreateCard(EntityType::GI));
	enemy_deck->AddCard(App->card_manager->CreateCard(EntityType::GI));
	enemy_deck->AddCard(App->card_manager->CreateCard(EntityType::GUARDIAN_GI));
	enemy_deck->AddCard(App->card_manager->CreateCard(EntityType::HARRIER));

	Deck* test_deck = new Deck();
	test_deck->delete_cards = true;
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::CONSCRIPT));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::SNIPER));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::FLAK_TROOPER));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::HARRIER));

	test_core = App->entity_manager->CreateCore(1, { 30,980 }, test_deck, FACTION_RUSSIAN);
	test_enemy_core = App->entity_manager->CreateCore(33, { 25,330 }, enemy_deck, FACTION_AMERICAN);
	test_core->LoadUnitSprites();
	test_enemy_core->LoadUnitSprites();

	do {
		random_num[0] = rand() % 9 + 1;
		random_num[1] = rand() % 9 + 1;
		random_num[2] = rand() % 9 + 1;
	} while (random_num[0] != random_num[1] != random_num[2]);

	//Initialize UI
	StartUI();

	return true;
}

// Called each loop iteration
bool TestingScene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p1 = App->render->ScreenToWorld(x, y);
	iPoint p2 = App->map->WorldToMap(p1.x, p1.y);

	LOG("casilla %i,%i   mouse world %i %i", p2.x, p2.y, p1.x,p1.y);

	return true;
}

// Called each loop iteration
bool TestingScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 100 * dt;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 100 * dt;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 100 * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 100 * dt;


	switch (state)
	{
	case TestingScene::BattleSceneState::SETUP:
	{
		//TODO TIMER ANIMATION ETC ETC
	}
		break;
	case TestingScene::BattleSceneState::FIGHT:
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		iPoint tile_p = App->map->WorldToMap(p.x, p.y);

		//-------SHORTCUTS-----------------------//

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				test_core->UseCard(CN_FIRST, { float(p.x),float(p.y) });
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				test_core->UseCard(CN_SECOND, { float(p.x),float(p.y) });
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				test_core->UseCard(CN_THIRD, { float(p.x),float(p.y) });
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				test_core->UseCard(CN_FOURTH, { float(p.x),float(p.y) });
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		{
			test_enemy_core->UseCard(CN_FIRST, { float(p.x),float(p.y) });
		}

		if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		{
			test_enemy_core->UseCard(CN_SECOND, { float(p.x),float(p.y) });
		}

		if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
		{
			test_enemy_core->UseCard(CN_THIRD, { float(p.x),float(p.y) });
		}

		if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
		{
			test_enemy_core->UseCard(CN_FOURTH, { float(p.x),float(p.y) });
		}


		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_UP)
		{
			state = BattleSceneState::WIN;
			App->PauseGame();
			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType() != EntityType::STORE_STRATEGY_BUILDING)App->gui->EnableElement((UIElement*)win_panel_one);
			else {
				//if (!App->game_manager->popups[POPUP_STORE])
				//	App->game_manager->ShowPopUp(POPUP_STORE);

			}

			App->gui->DisableInteractable((UIElement*)unit_panel);
			App->game_manager->GetEncounterTree()->SetCurrentNode(App->game_manager->GetEncounterTree()->GetFightingNode());
			App->game_manager->gold += 100;
			App->game_manager->enemy_scaling += 1;

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType() != EntityType::STORE_STRATEGY_BUILDING)
			{
				App->gui->EnableElement((UIElement*)win_panel_one);
				App->game_manager->LevelUpgrade();
			}

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetChildren().size() == 0) {
				if (App->game_manager->stage == STAGE_TUTORIAL) App->game_manager->stage++;
			}
		}

		if (!test_core->IsAlive())
		{
			state = BattleSceneState::LOSE;
			App->PauseGame();
		}
		else if (!test_enemy_core->IsAlive())
		{
			state = BattleSceneState::WIN;
			App->PauseGame();
			App->gui->EnableElement((UIElement*)win_panel_one);
			App->gui->DisableInteractable((UIElement*)unit_panel);
		}
	}
		break;
	case TestingScene::BattleSceneState::WIN:
	{


	}
		break;
	case TestingScene::BattleSceneState::LOSE:
	{
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
	}
		break;
	default:
		break;
	}

	return true;
}

// Called each loop iteration
bool TestingScene::PostUpdate()
{
	bool ret = true;

	App->map->Draw();

	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);


	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (current_drag)
		current_drag->GetScreenPos();

	return ret;
}

// Called before quitting
bool TestingScene::CleanUp()
{
	LOG("Freeing scene");

	App->tex->UnLoad(debug_tex);

	return true;
}

bool TestingScene::GUIEvent(UIElement * element, GUI_Event gui_event)
{
	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {
		if (element == unit_button_one) {
			CreateDrag(CN_FIRST, test_core->GetCard(CN_FIRST)->type, element);
		}
		else if (element == unit_button_two) {
			CreateDrag(CN_SECOND, test_core->GetCard(CN_SECOND)->type, element);
		}
		else if (element == unit_button_three) {
			CreateDrag(CN_THIRD, test_core->GetCard(CN_THIRD)->type, element);
		}
		else if (element == unit_button_four) {
			CreateDrag(CN_FOURTH, test_core->GetCard(CN_FOURTH)->type, element);
		}
		else if (element == win_continue_one) {
			App->gui->DisableElement((UIElement*)win_panel_one);
			App->gui->EnableElement((UIElement*)win_panel_two);
		}
			
	}
	else if (gui_event == GUI_Event::LEFT_CLICK_UP) {
		if (element == current_drag) {
			ReleaseDrag();
		}		
	}

	return true;
}

void TestingScene::StartUI()
{
	//Game_UI

	unit_panel = App->gui->CreateImage({ 755,0 }, { 619,0,269,768 });
	unit_button_one = App->gui->CreateButton({ 35, 365 }, App->gui->LoadUIButton(test_core->GetCard(CN_FIRST)->type, "button"), unit_panel);
	unit_button_two = App->gui->CreateButton({ 135, 365 }, App->gui->LoadUIButton(test_core->GetCard(CN_SECOND)->type, "button"), unit_panel);
	unit_button_three = App->gui->CreateButton({ 35, 445 }, App->gui->LoadUIButton(test_core->GetCard(CN_THIRD)->type, "button"), unit_panel);
	unit_button_four = App->gui->CreateButton({ 135, 445 }, App->gui->LoadUIButton(test_core->GetCard(CN_FOURTH)->type, "button"), unit_panel);

	energy_bar = App->gui->CreateBar({ 764, 358 }, { 601,0,16,274 }, test_core->GetEnergy());

	health_bar_image = App->gui->CreateImage({ 470,730 }, { 747,1215,353,28 });
	health_bar = App->gui->CreateBar({ 498,740 }, { 747,1244,223,16 }, test_core->GetHealth(), BarType::BAR_HORITZONTAL);

	// End Game Screen
	SDL_Rect button_rect[3];
	button_rect[0] = { 221,533,220,51 };
	button_rect[1] = { 221,585,220,51 };
	button_rect[2] = { 221,637,220,51 };

	win_panel_one = App->gui->CreateImage({ 139,150 }, { 3986,1646,605,660 });
	win_panel_two = App->gui->CreateImage({ 139,150 }, { 1,852,744,466 });
	win_text_one = App->gui->CreateLabel({ 30,30 }, "fonts/red_alert.ttf", 40, "Congratulations, you've conquered this zone and unlocked the next building!", { 255,232,2, 255 }, 710, win_panel_one);
	win_text_two = App->gui->CreateLabel({ 30,30 }, "fonts/red_alert.ttf", 40, "Upgrade a troop or choose a new one to add to your deck", { 255,232,2, 255 }, 710, win_panel_two);
	win_continue_one = App->gui->CreateButton({ 262,375 }, button_rect, win_panel_one);
	win_continue_two = App->gui->CreateButton({ 262,375 }, button_rect, win_panel_two);

	win_unit_one = App->gui->CreateButton({ 130,200 }, App->gui->LoadUIButton(random_num[0],"upgrade"), win_panel_two, true);
	win_unit_two = App->gui->CreateButton({ 320,200 }, App->gui->LoadUIButton(random_num[1], "upgrade"), win_panel_two, true);
	win_unit_three = App->gui->CreateButton({ 510,200 }, App->gui->LoadUIButton(random_num[2], "upgrade"), win_panel_two, true);

	win_building = App->gui->CreateImage({ 300,200 }, App->gui->LoadUIImage(13, "end_screen"), win_panel_one);

	App->gui->DisableElement((UIElement*)win_panel_one);
	App->gui->DisableElement((UIElement*)win_panel_two);
	App->gui->EnableInteractable((UIElement*)unit_panel);


}

void TestingScene::CreateDrag(int num, int type, UIElement* element)
{
	card_num = num;
	current_drag = App->gui->CreateImage({ 0,0 }, App->gui->LoadUIButton(type, "drag")[0], element);
	current_drag->interactable = true;
	current_drag->dragable = true;
	current_drag->clipping = false;
	current_drag->parent_limit = false;
	current_drag->clicked = true;
	App->map->SetDrawable("Spawn", 0);
}

void TestingScene::ReleaseDrag()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint world_pos = App->render->ScreenToWorld(x, y);
	iPoint map_pos = App->map->WorldToMap(world_pos.x, world_pos.y);

	if (App->map->IsInsideMap(map_pos) && App->map->IsSpawnable(map_pos))
	{
		test_core->UseCard(card_num, { float(world_pos.x),float(world_pos.y) });
	}

	App->map->SetDrawable("Spawn", 1);
	App->gui->DeleteElement(current_drag);
	current_drag = nullptr;
}