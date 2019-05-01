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
#include "PathFinding.h"
#include "UIAnimatedImage.h"
#include "UIButton.h"
#include "UISelectableButton.h"
#include "UIBar.h"
#include "GUI.h"
#include "SceneManager.h"
#include "TransitionManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "BattleScene.h"
#include "GameManager.h"
#include "EncounterTree.h"
#include "EncounterNode.h"
#include "UIImage.h"

const double HELD_DELAY = 175;

BattleScene::BattleScene() : Scene()
{

}

// Destructor
BattleScene::~BattleScene()
{}

// Called before the first frame
bool BattleScene::Start()
{
	if (App->map->Load("test_grande.tmx") == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	App->render->camera.x = (App->map->data.width*App->map->data.tile_width*0.5)*0.5 - 250;
	App->render->camera.y = -210;

	Deck* enemy_deck = new Deck();
	enemy_deck->delete_cards = true;
	enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDeck()[0]));
	enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDeck()[1]));
	enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDeck()[2]));
	enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDeck()[3]));

	allied_core = App->entity_manager->CreateCore(1, { 30,980 }, App->game_manager->GetPlayerDeck(), FACTION_RUSSIAN);
	enemy_core = App->entity_manager->CreateCore(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType(), { 25,330 }, enemy_deck, FACTION_AMERICAN, true);
	enemy_core->delete_deck = true;

	allied_core->LoadUnitSprites();
	enemy_core->LoadUnitSprites();

	//Initialize UI
	StartUI();

	win_fx = App->audio->LoadFx("audio/fx/Mission/Mission_accomplished.wav");
	lose_fx = App->audio->LoadFx("audio/fx/Mission/Mission_Failed.wav");
	deployment_fx = App->audio->LoadFx("audio/fx/Voice_Over/Unit_ready.wav");
	no_energy = App->audio->LoadFx("audio/fx/UI/gpsyampa.wav");
	App->audio->PlayMusic("audio/music/9.Destroy-Red Alert2_2.ogg");

	return true;
}

// Called each loop iteration
bool BattleScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool BattleScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	switch (state)
	{
	case BattleScene::BattleSceneState::SETUP:
	{
		//TODO TIMER ANIMATION ETC ETC
	}
	break;
	case BattleScene::BattleSceneState::FIGHT:
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		iPoint tile_p = App->map->WorldToMap(p.x, p.y);

		//-------SHORTCUTS-----------------------//
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN
			|| App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		{
			shortcut_timer.Start();
		}

		if ((App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT
			|| App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT)
			&& shortcut_timer.ReadMs() > HELD_DELAY)
		{
			App->map->SetDrawable("Spawn", 0);
		}

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_UP)
		{
			App->map->SetDrawable("Spawn", 1);
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				if (allied_core->UseCard(CN_FIRST, { float(p.x),float(p.y) }))
					App->audio->PlayFx(deployment_fx.c_str(), 0);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP)
		{
			App->map->SetDrawable("Spawn", 1);
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				if (allied_core->UseCard(CN_SECOND, { float(p.x),float(p.y) }))
					App->audio->PlayFx(deployment_fx.c_str(), 0);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_UP)
		{
			App->map->SetDrawable("Spawn", 1);
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				if (allied_core->UseCard(CN_THIRD, { float(p.x),float(p.y) }))
					App->audio->PlayFx(deployment_fx.c_str(), 0);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_4) == KEY_UP)
		{
			App->map->SetDrawable("Spawn", 1);
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				if (allied_core->UseCard(CN_FOURTH, { float(p.x),float(p.y) }))
					App->audio->PlayFx(deployment_fx.c_str(), 0);
			}
		}
			

		//---------------------------------------//

		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_UP)
		{
			state = BattleSceneState::WIN;
			App->PauseGame();
			App->gui->EnableElement((UIElement*)win_panel_one);
			App->audio->PlayFx(win_fx.c_str(), 0);
			App->game_manager->GetEncounterTree()->SetCurrentNode(App->game_manager->GetEncounterTree()->GetFightingNode());
		}
			


		if (!allied_core->IsAlive())
		{
			state = BattleSceneState::LOSE;
			App->audio->PlayFx(lose_fx.c_str(), 0);
			App->PauseGame();
			App->gui->EnableElement((UIElement*)lose_panel);
			App->gui->DisableInteractable((UIElement*)unit_panel);
		}
		else if (!enemy_core->IsAlive())
		{
			state = BattleSceneState::WIN;
			App->audio->PlayFx(win_fx.c_str(), 0);
			App->PauseGame();
			App->gui->EnableElement((UIElement*)win_panel_one);
			App->gui->DisableInteractable((UIElement*)unit_panel);
			App->game_manager->GetEncounterTree()->SetCurrentNode(App->game_manager->GetEncounterTree()->GetFightingNode());
		}
	}
	break;
	case BattleScene::BattleSceneState::WIN:
	{
			
	}
	break;
	case BattleScene::BattleSceneState::LOSE:
	{

	}
	break;
	default:
		break;
	}

	return true;
}

// Called each loop iteration
bool BattleScene::PostUpdate()
{
	bool ret = true;

	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool BattleScene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool BattleScene::GUIEvent(UIElement * element, GUI_Event gui_event)
{
	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {
		if (element == unit_button_one) {
			CreateDrag(CN_FIRST, allied_core->GetCard(CN_FIRST)->type, element);
		}
		else if (element == unit_button_two) {
			CreateDrag(CN_SECOND, allied_core->GetCard(CN_SECOND)->type, element);
		}
		else if (element == unit_button_three) {
			CreateDrag(CN_THIRD, allied_core->GetCard(CN_THIRD)->type, element);
		}
		else if (element == unit_button_four) {
			CreateDrag(CN_FOURTH, allied_core->GetCard(CN_FOURTH)->type, element);
		}
		else if (element == win_continue_one) {
			App->gui->DisableElement((UIElement*)win_panel_one);
			App->gui->EnableElement((UIElement*)win_panel_two);
		}
		else if (element == win_continue_two) {
			if (win_unit_one->IsSelected()) {
				App->game_manager->AddCardToCollection((EntityType)random_num[0]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
			}
			else if (win_unit_two->IsSelected()) {
				App->game_manager->AddCardToCollection((EntityType)random_num[1]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
			}
			else if (win_unit_three->IsSelected()) {
				App->game_manager->AddCardToCollection((EntityType)random_num[2]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
			}
		}
		else if (element == lose_continue) {
			App->gui->DisableElement((UIElement*)lose_panel);
			App->game_manager->restart = true;
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
		}

		if (element == win_unit_one) {
			win_unit_two->ChangeState(false);
			win_unit_three->ChangeState(false);
		}
		else if (element == win_unit_two) {
			win_unit_one->ChangeState(false);
			win_unit_three->ChangeState(false);
		}
		else if (element == win_unit_three) {
			win_unit_two->ChangeState(false);
			win_unit_one->ChangeState(false);
		}
	}
	else if (gui_event == GUI_Event::LEFT_CLICK_UP) {
		if (element == current_drag) {
			ReleaseDrag();
		}
	}

	return true;
}

void BattleScene::CreateDrag(int num, int type, UIElement* element)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	x = x - element->GetScreenPos().x;
	y = y - element->GetScreenPos().y;

	card_num = num;
	current_drag = App->gui->CreateImage({ x,y }, App->gui->LoadUIButton(type, "drag")[0], element);
	current_drag->interactable = true;
	current_drag->dragable = true;
	current_drag->clipping = false;
	current_drag->parent_limit = false;
	current_drag->clicked = true;
	current_drag->hovered = true;
	App->map->SetDrawable("Spawn", 0);
}

void BattleScene::ReleaseDrag()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint world_pos = App->render->ScreenToWorld(x, y);
	iPoint map_pos = App->map->WorldToMap(world_pos.x, world_pos.y);

	if (App->map->IsInsideMap(map_pos) && App->map->IsSpawnable(map_pos))
	{
		if (allied_core->UseCard(card_num, { float(world_pos.x),float(world_pos.y) }))
		{ 
			App->audio->PlayFx(deployment_fx.c_str(), 0);
		}
		else App->audio->PlayFx(no_energy.c_str(), 0);
	}

	

	App->map->SetDrawable("Spawn", 1);
	App->gui->DeleteElement(current_drag);
	current_drag = nullptr;
}

void BattleScene::StartUI()
{
	//Generate random number
	do {
		random_num[0] = rand() % 18 + 1;
		random_num[1] = rand() % 18 + 1;
		random_num[2] = rand() % 18 + 1;
	} while ((random_num[0] == random_num[1] || random_num[0] == random_num[2] || random_num[1] == random_num[2])
		|| (random_num[0] % 2 == 0 || random_num[1] % 2 == 0 || random_num[2] % 2 == 0));

	//Game_UI

	unit_panel = App->gui->CreateImage({ 755,0 }, { 619,0,269,768 });
	unit_button_one = App->gui->CreateButton({ 35, 365 }, App->gui->LoadUIButton(allied_core->GetCard(CN_FIRST)->type, "button"), unit_panel);
	unit_button_two = App->gui->CreateButton({ 135, 365 }, App->gui->LoadUIButton(allied_core->GetCard(CN_SECOND)->type, "button"), unit_panel);
	unit_button_three = App->gui->CreateButton({ 35, 445 }, App->gui->LoadUIButton(allied_core->GetCard(CN_THIRD)->type, "button"), unit_panel);
	unit_button_four = App->gui->CreateButton({ 135, 445 }, App->gui->LoadUIButton(allied_core->GetCard(CN_FOURTH)->type, "button"), unit_panel);

	energy_bar = App->gui->CreateBar({ 764, 358 }, { 601,0,16,274 }, allied_core->GetEnergy());

	health_bar_image = App->gui->CreateImage({ 470,730 }, { 25,1399,253,28 });
	enemy_health_bar_image = App->gui->CreateImage({ 40,20 }, { 25,1474,253,28 });
	health_bar = App->gui->CreateBar({ 28,10 }, { 25,1428,224,16 }, allied_core->GetHealth(), BarType::BAR_HORITZONTAL, nullptr, health_bar_image);
	enemy_health_bar = App->gui->CreateBar({ 28,10 }, { 25,1428,224,16 }, enemy_core->GetHealth(), BarType::BAR_HORITZONTAL, nullptr, enemy_health_bar_image);

	App->gui->EnableInteractable((UIElement*)unit_panel);

	// End Game Screen Win
	SDL_Rect button_rect[3];
	button_rect[0] = { 221,533,220,51 };
	button_rect[1] = { 221,585,220,51 };
	button_rect[2] = { 221,637,220,51 };

	win_panel_one = App->gui->CreateImage({ 139,150 }, { 1,852,744,466 });
	win_panel_two = App->gui->CreateImage({ 139,150 }, { 1,852,744,466 });
	win_text_one = App->gui->CreateLabel({ 30,30 }, "fonts/red_alert.ttf", 40, "Congratulations, you've conquered this zone and unlocked the next building!", { 255,232,2, 255 }, 710, win_panel_one);
	win_text_two = App->gui->CreateLabel({ 30,30 }, "fonts/red_alert.ttf", 40, "Upgrade a troop or choose a new one to add to your deck", { 255,232,2, 255 }, 710, win_panel_two);
	win_continue_one = App->gui->CreateButton({ 262,375 }, button_rect, win_panel_one);
	win_continue_two = App->gui->CreateButton({ 262,375 }, button_rect, win_panel_two);

	win_unit_one = App->gui->CreateSelectableButton({ 130,200 }, App->gui->LoadUIButton(random_num[0], "upgrade"), win_panel_two);
	win_unit_two = App->gui->CreateSelectableButton({ 320,200 }, App->gui->LoadUIButton(random_num[1], "upgrade"), win_panel_two);
	win_unit_three = App->gui->CreateSelectableButton({ 510,200 }, App->gui->LoadUIButton(random_num[2], "upgrade"), win_panel_two);

	win_building = App->gui->CreateImage({ 260,160 }, App->gui->LoadUIImage(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType()), win_panel_one);

	App->gui->DisableElement((UIElement*)win_panel_one);
	App->gui->DisableElement((UIElement*)win_panel_two);

	//End Game Screen Lose
	lose_panel = App->gui->CreateImage({ 139,150 }, { 1,852,744,466 });
	lose_text = App->gui->CreateLabel({ 30,30 }, "fonts/red_alert.ttf", 40, "The enemy troops have defeat yours! Allies have win the battle", { 255,232,2, 255 }, 710, lose_panel);
	lose_continue = App->gui->CreateButton({ 262,375 }, button_rect, lose_panel);

	App->gui->DisableElement((UIElement*)lose_panel);

}
