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
#include "UIButtonText.h"
#include "UISelectableButton.h"
#include "UIBar.h"
#include "GUI.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "TransitionManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "Stat.h"
#include "BattleScene.h"
#include "Buff.h"
#include "GameManager.h"
#include "Particles.h"
#include "EncounterTree.h"
#include "EncounterNode.h"
#include "UIImage.h"
#include "UILabel.h"

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

	App->render->camera.x = 290;
	App->render->camera.y = -635;

	Deck* enemy_deck = new Deck();
	enemy_deck->delete_cards = true;
	for (int i = 0; i < App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDeck().size(); i++)
	{
		enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDeck()[i]));
	}

	SetEnemiesUpgrades(enemy_deck);

	iPoint allied_core_world = App->map->MapToWorld(51, 50);
	iPoint enemy_core_world = App->map->MapToWorld(27, 26);

	allied_core = App->entity_manager->CreateCore(1, { (float)allied_core_world.x,  (float)allied_core_world.y }, App->game_manager->GetPlayerDeck(), FACTION_RUSSIAN);
	enemy_core = App->entity_manager->CreateCore(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType(), { (float)enemy_core_world.x,  (float)enemy_core_world.y }, enemy_deck, FACTION_AMERICAN, true);
	enemy_core->delete_deck = true;

	allied_core->LoadUnitSprites(App->game_manager->GetPlayerDeck()->GetDeckSize());
	enemy_core->LoadUnitSprites();

	App->game_manager->health_upgrade->GetBuffs(allied_core->stats);
	App->game_manager->energy_upgrade->GetBuffs(allied_core->stats);


	allied_core->DecreaseEnergy(20);

	//Initialize UI
	StartUI();

	win_fx = App->audio->LoadFx("audio/fx/Mission/Mission_accomplished.wav");
	lose_fx = App->audio->LoadFx("audio/fx/Mission/Mission_Failed.wav");
	deployment_fx = App->audio->LoadFx("audio/fx/Voice_Over/Unit_ready.wav");
	no_energy = App->audio->LoadFx("audio/fx/UI/gpsyampa.wav");
	App->audio->PlayMusic("audio/music/9.Destroy-Red Alert2_2.ogg");

	App->audio->SetAllFXVolume(App->audio->GetFxVolume());

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

	if (!App->game_manager->popups[POPUP_USETROOP])
	{
		if (!App->transition_manager->IsTransitioning())
			App->game_manager->ShowPopUp(POPUP_USETROOP);
	}

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

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_UP && allied_core->GetCard(CN_FIRST))
		{
			App->map->SetDrawable("Spawn", 1);
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				if (allied_core->UseCard(CN_FIRST, { float(p.x),float(p.y) }))
					App->audio->PlayFx(deployment_fx.c_str(), 0);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_UP && allied_core->GetCard(CN_SECOND))
		{
			App->map->SetDrawable("Spawn", 1);
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				if (allied_core->UseCard(CN_SECOND, { float(p.x),float(p.y) }))
					App->audio->PlayFx(deployment_fx.c_str(), 0);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_UP && allied_core->GetCard(CN_THIRD))
		{
			App->map->SetDrawable("Spawn", 1);
			if (App->map->IsInsideMap(tile_p) && App->map->IsSpawnable(tile_p))
			{
				if (allied_core->UseCard(CN_THIRD, { float(p.x),float(p.y) }))
					App->audio->PlayFx(deployment_fx.c_str(), 0);
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_4) == KEY_UP && allied_core->GetCard(CN_FOURTH))
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
			App->audio->PlayFx(win_fx.c_str(), 0);
			App->PauseGame();
			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType() != EntityType::STORE_STRATEGY_BUILDING)App->gui->EnableElement((UIElement*)win_panel_one);
			else {
				//if (!App->game_manager->popups[POPUP_STORE])
				//	App->game_manager->ShowPopUp(POPUP_STORE);

				App->gui->EnableElement((UIElement*)store_panel);
			}

			App->gui->DisableInteractable((UIElement*)unit_panel);
			App->game_manager->GetEncounterTree()->SetCurrentNode(App->game_manager->GetEncounterTree()->GetFightingNode());
			App->game_manager->gold += 100;
			App->game_manager->enemy_scaling += 1;

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType() != EntityType::STORE_STRATEGY_BUILDING)
			{
				App->gui->EnableElement((UIElement*)win_panel_one);
				App->game_manager->LevelUpgrade((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType());
			}
			else 
			{
				App->gui->EnableElement((UIElement*)store_panel);
				current_gold->SetText("Your gold: " + std::to_string(App->game_manager->gold));
			}

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetChildren().size() == 0) {
				if (App->game_manager->stage == STAGE_TUTORIAL) App->game_manager->stage++;
			}
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
			App->gui->DisableInteractable((UIElement*)unit_panel);
			App->game_manager->GetEncounterTree()->SetCurrentNode(App->game_manager->GetEncounterTree()->GetFightingNode());
			App->game_manager->gold += 100;
			App->game_manager->enemy_scaling += 1;

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType() != EntityType::STORE_STRATEGY_BUILDING)
			{
				App->gui->EnableElement((UIElement*)win_panel_one);
				App->game_manager->LevelUpgrade((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType());
			}
			else
			{
				App->gui->EnableElement((UIElement*)store_panel);
				current_gold->SetText("Your gold: " + std::to_string(App->game_manager->gold));
			}

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetChildren().size() == 0) {
				if (App->game_manager->stage == STAGE_TUTORIAL)App->game_manager->stage++;
			}
		}

		energy_label->SetText(std::to_string(energy_bar->GetValue()));
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
				App->game_manager->AddCardToCollection((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[0]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
			}
			else if (win_unit_two->IsSelected()) {
				App->game_manager->AddCardToCollection((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[1]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
			}
			else if (win_unit_three->IsSelected()) {
				App->game_manager->AddCardToCollection((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[2]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
			}

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetChildren().size() == 0) {
				App->game_manager->ResetBuildingBuffs();
				App->game_manager->GetEncounterTree()->CleanTree();
				App->game_manager->CreateStage();
				App->game_manager->SaveRecoveryState();
			}
			App->SaveGame(nullptr);
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

		if (element == store_unit_one) {
			if(store_unit_one->selected)UpdateGoldOnSelect(random_store_unit[0]);
			else UpdateGoldOnUnSelect(random_store_unit[0]);
		}
		if (element == store_unit_two) {
			if (store_unit_two->selected)UpdateGoldOnSelect(random_store_unit[1]);
			else UpdateGoldOnUnSelect(random_store_unit[1]);
		}
		if (element == store_unit_three) {
			if (store_unit_three->selected)UpdateGoldOnSelect(random_store_unit[2]);
			else UpdateGoldOnUnSelect(random_store_unit[2]);
		}
		if (element == store_unit_four) {
			if (store_unit_four->selected)UpdateGoldOnSelect(random_store_unit[3]);
			else UpdateGoldOnUnSelect(random_store_unit[3]);
		}
		if (element == store_unit_five) {
			if (store_unit_five->selected)UpdateGoldOnSelect(random_store_unit[4]);
			else UpdateGoldOnUnSelect(random_store_unit[4]);
		}
		if (element == store_unit_six) {
			if (store_unit_six->selected)UpdateGoldOnSelect(random_store_unit[5]);
			else UpdateGoldOnUnSelect(random_store_unit[5]);
		}

		if (element == purchase) {
			for each (EntityType et in store_units_purchased)
			{
				App->game_manager->AddCardToCollection(et);
			}
			App->gui->DisableElement((UIElement*)store_panel);

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetChildren().size() == 0) {
				App->game_manager->ResetBuildingBuffs();
				App->game_manager->GetEncounterTree()->CleanTree();
				App->game_manager->CreateStage();
				App->game_manager->SaveRecoveryState();
			}

			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
		}

		if (element == bomb_button)
		{
			DropNukes();
			bomb_button->SetLocked(false);
		}


		if (element == pause_button) {
			if (!App->IsPaused()) {
				App->PauseGame();
				App->gui->EnableElement(pause_panel);
			}
			else {
				App->ResumeGame();
				App->gui->DisableElement(pause_panel);
			}
		}

		if (element == pause_continue && App->IsPaused()) {
			App->ResumeGame();
			App->gui->DisableElement(pause_panel);
		}
		else if (element == pause_exit) {
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MENU, White);
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

void BattleScene::GenerateRandomAlliedTroop()
{
	std::vector<int> pool = { 1, 3, 5, 7, 9, 11, 13, 15, 17 };
	std::vector<int>::iterator it;

	for (int i = 0; i < 6; i++)
	{
		int position = rand() % pool.size();
		int card = pool[position];
		random_store_unit.push_back(card);
		it = pool.begin() + position;
		pool.erase(it);
	}
}

void BattleScene::UpdateGoldOnSelect(int unit)
{
	total_cost_acumulated += 100;
	App->game_manager->gold -= 100;
	current_gold->SetText("Your gold: " + std::to_string(App->game_manager->gold));
	total_cost->SetText("Total cost: " + std::to_string(total_cost_acumulated));

	if (App->game_manager->gold < 0)
	{
		current_gold->SetColor({ 255, 0, 0, 255 });
		purchase->interactable = false;
		purchase->SetTextColor({ 255,0,0,255 });
	}
	else {
		current_gold->SetColor({ 255,232,2, 255 });
		purchase->interactable = true;
		purchase->SetTextColor({ 255,232,2, 255 });
	}

	store_units_purchased.push_back((EntityType)unit);
	if (store_units_purchased.size() > 0)purchase->SetText("PURCHASE");
}

void BattleScene::UpdateGoldOnUnSelect(int unit)
{
	total_cost_acumulated -= 100;
	App->game_manager->gold += 100;
	current_gold->SetText("Your gold: " + std::to_string(App->game_manager->gold));
	total_cost->SetText("Total cost: " + std::to_string(total_cost_acumulated));

	if (App->game_manager->gold < 0)
	{
		current_gold->SetColor({ 255, 0, 0, 255 });
		purchase->interactable = false;
		purchase->SetTextColor({ 255,0,0,255 });
	}
	else {
		current_gold->SetColor({ 255,232,2, 255 });
		purchase->interactable = true;
		purchase->SetTextColor({ 255,232,2, 255 });
	}

	store_units_purchased.remove((EntityType)unit);

	if (store_units_purchased.size() == 0)purchase->SetText("CONTINUE");
}

void BattleScene::SetEnemiesUpgrades(Deck* enemy_deck)
{
	int aux = 0;
	for (int i = 0; i < App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDifficulty(); i++)
	{
		enemy_deck->cards[aux]->Upgrade();
		aux++;
		if (aux > 3)aux = 0;
	}

	if(enemy_deck->cards[1] && enemy_deck->cards[2] && enemy_deck->cards[3])
	LOG("enemy cards level %i, %i, %i, %i", enemy_deck->cards[0]->level, enemy_deck->cards[1]->level, enemy_deck->cards[2]->level, enemy_deck->cards[3]->level);
}

void BattleScene::DropNukes()
{
	App->particles->CreateParticle(ParticleType::NUKE_BOMB, { allied_core->position.x - (allied_core->current_frame.w * 2), 0 },
		{ allied_core->position.x - (allied_core->current_frame.w * 2), allied_core->position.y - allied_core->current_frame.h }, 140);
	App->particles->CreateParticle(ParticleType::NUKE_BOMB, { allied_core->position.x, 0 }, 
		{ allied_core->position.x, allied_core->position.y - (allied_core->current_frame.h * 1.75f) }, 130);
	App->particles->CreateParticle(ParticleType::NUKE_BOMB, { allied_core->position.x + (allied_core->current_frame.w * 2), 0 },
		{ allied_core->position.x + (allied_core->current_frame.w * 2), allied_core->position.y - allied_core->current_frame.h }, 140);
}

void BattleScene::StartUI()
{
	//Generate random number
	GenerateRandomAlliedTroop();
	//Game_UI

	uint width, height;
	App->win->GetWindowSize(width, height);

	side_troop_panel = App->gui->CreateImage({ 522, 835 }, { 1431,383,108,130 }, nullptr);
	side_troop_panel_left = App->gui->CreateImage({ 10, 885 }, { 1431,383,108,130 }, nullptr);

	unit_panel = App->gui->CreateImage({ 80, (int)height-145}, { 1231,186,481,155 });
	if (allied_core->GetCard(CN_FIRST)) {
		unit_button_one = App->gui->CreateButton({ 27, 52 }, App->gui->LoadUIButton(allied_core->GetCard(CN_FIRST)->type, "button"), unit_panel);
		energy_cost[0] = App->gui->CreateImage({ -8, 60 }, { 1282,349,25,25 }, unit_button_one);
		energy_cost_label[0] = App->gui->CreateLabel({ 7,2 }, "fonts/gunplay.ttf", 18, std::to_string((int)allied_core->GetCard(CN_FIRST)->info.stats.find("energy_cost")->second->GetValue()), { 255,255,255,255 }, 120, energy_cost[0], false);
	}
	if (allied_core->GetCard(CN_SECOND))
	{
		unit_button_two = App->gui->CreateButton({ 136, 52 }, App->gui->LoadUIButton(allied_core->GetCard(CN_SECOND)->type, "button"), unit_panel);
		energy_cost[1] = App->gui->CreateImage({ -8, 60 }, { 1282,349,25,25 }, unit_button_two);
		energy_cost_label[1] = App->gui->CreateLabel({ 7,2 }, "fonts/gunplay.ttf", 18, std::to_string((int)allied_core->GetCard(CN_SECOND)->info.stats.find("energy_cost")->second->GetValue()), { 255,255,255,255 }, 120, energy_cost[1], false);
	}
	if (allied_core->GetCard(CN_THIRD)) {
		unit_button_three = App->gui->CreateButton({ 245, 52 }, App->gui->LoadUIButton(allied_core->GetCard(CN_THIRD)->type, "button"), unit_panel);
		energy_cost[2] = App->gui->CreateImage({ -8, 60 }, { 1282,349,25,25 }, unit_button_three);
		energy_cost_label[2] = App->gui->CreateLabel({ 7,2 }, "fonts/gunplay.ttf", 18, std::to_string((int)allied_core->GetCard(CN_THIRD)->info.stats.find("energy_cost")->second->GetValue()), { 255,255,255,255 }, 120, energy_cost[2], false);
	}
	if (allied_core->GetCard(CN_FOURTH)) {
		unit_button_four = App->gui->CreateButton({ 354, 52 }, App->gui->LoadUIButton(allied_core->GetCard(CN_FOURTH)->type, "button"), unit_panel);
		energy_cost[3] = App->gui->CreateImage({ -8, 60 }, { 1282,349,25,25 }, unit_button_four);
		energy_cost_label[3] = App->gui->CreateLabel({ 7,2 }, "fonts/gunplay.ttf", 18, std::to_string((int)(allied_core->GetCard(CN_FOURTH)->info.stats.find("energy_cost")->second->GetValue())), { 255,255,255,255 }, 120, energy_cost[3], false);
	}

	energy_bar = App->gui->CreateBar({ 30, 11 }, { 1237,141,446,36 }, allied_core->GetEnergy(), BAR_HORITZONTAL, BAR_DYNAMIC, nullptr, unit_panel);
	energy_image = App->gui->CreateImage({ 8, 10 }, { 1238,345,32,32 }, unit_panel);
	energy_label = App->gui->CreateLabel({ 10,4 }, "fonts/gunplay.ttf", 20, "0", { 255,255,255,255 }, 120, energy_image, false);

	SDL_Rect pause_rect[3];
	pause_rect[0] = { 1168,1371,44,36 };
	pause_rect[1] = { 1215,1371,44,36 };
	pause_rect[2] = { 1262,1371,44,36 };
	pause_button = App->gui->CreateButton({ 25, 910 }, pause_rect, nullptr);

	SDL_Rect bomb_button_rect[3];
	bomb_button_rect[0] = { 1316,1304,58,57 };
	bomb_button_rect[1] = { 1376,1303,58,57 };
	bomb_button_rect[2] = { 1434,1303,58,57 };
	bomb_button = App->gui->CreateButton({ 567, 847 }, bomb_button_rect, nullptr);

	SDL_Rect faction_button_rect[3];
	faction_button_rect[0] = { 1346,1366,39,39};
	faction_button_rect[1] = { 1387,1366,39,39 };
	faction_button_rect[2] = { 1427,1366,39,39 };
	faction_button = App->gui->CreateButton({ 575, 912 }, faction_button_rect, nullptr);

	health_bar_image = App->gui->CreateImage({ 248, 770 }, { 24,1378,144,16 });
	enemy_health_bar_image = App->gui->CreateImage({ 248, 30 }, { 24,1455,144,16 });
	health_bar = App->gui->CreateBar({ 16,5 }, { 24,1404,127,10 }, allied_core->GetHealth(), BarType::BAR_HORITZONTAL, BAR_DYNAMIC, nullptr, health_bar_image);
	enemy_health_bar = App->gui->CreateBar({ 16,6 }, { 24,1404,127,10 }, enemy_core->GetHealth(), BarType::BAR_HORITZONTAL, BAR_DYNAMIC, nullptr, enemy_health_bar_image);

	App->gui->EnableInteractable((UIElement*)unit_panel);

	// End Game Screen Win
	SDL_Rect button_rect[4];
	button_rect[0] = { 800,499,294,67 };
	button_rect[1] = { 800,569,294,67 };
	button_rect[2] = { 800,639,294,67 };
	button_rect[3] = { 800,639,294,67 };

	SDL_Rect purchase_rect[4];
	purchase_rect[0] = {2795, 1536, 220, 51};
	purchase_rect[1] = {2795, 1588, 220, 51};
	purchase_rect[2] = {2795, 1640, 220, 51};
	purchase_rect[3] = {2795, 1692, 220, 51};

	win_panel_one = App->gui->CreateImage({ 17,120 }, { 3986,1646,605,660 });
	win_panel_two = App->gui->CreateImage({ 17,120 }, { 3986,1646,605,660 });
	win_text_one = App->gui->CreateLabel({ 30,30 }, "fonts/red_alert.ttf", 40, "Congratulations, you've conquered this zone and unlocked the next building!", { 255,232,2, 255 }, 600, win_panel_one);
	win_text_two = App->gui->CreateLabel({ 30,30 }, "fonts/red_alert.ttf", 40, "Upgrade a troop or choose a new one to add to your deck", { 255,232,2, 255 }, 600, win_panel_two);
	win_continue_one = App->gui->CreateButtonText({ 170, 560 }, { 30,0}, button_rect, "CONTINUE", { 200,200,200,255 }, 27, win_panel_one);
	win_continue_two = App->gui->CreateButtonText({ 170, 560 }, { 30,0 }, button_rect, "CONTINUE", { 200,200,200,255 }, 27, win_panel_two);

	win_unit_one = App->gui->CreateSelectableButton({ 160,200 }, App->gui->LoadUIButton(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[0], "upgrade"), win_panel_two);
	win_unit_two = App->gui->CreateSelectableButton({ 350,200 }, App->gui->LoadUIButton(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[1], "upgrade"), win_panel_two);
	win_unit_three = App->gui->CreateSelectableButton({ 260,370 }, App->gui->LoadUIButton(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[2], "upgrade"), win_panel_two);

	win_building = App->gui->CreateImage({ 195,240 }, App->gui->LoadUIImage(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType(), "end_screen"), win_panel_one);

	App->gui->DisableElement((UIElement*)win_panel_one);
	App->gui->DisableElement((UIElement*)win_panel_two);


	//Pause
	pause_panel = App->gui->CreateImage({ 2, 87 }, { 3967,961,636,671 });
	options_label = App->gui->CreateLabel({ 180, 130 }, "fonts/button_text.ttf", 40, "Options", { 255,255,255,255 }, 400, pause_panel);
	pause_continue = App->gui->CreateButtonText({ 180, 480 }, { 37,5 }, button_rect, "CONTINUE", {243,242,153,255}, 27, pause_panel);
	pause_exit = App->gui->CreateButtonText({ 180, 590 }, { 14,5 }, button_rect, "BACK TO MENU", { 234,132,132,255 }, 22, pause_panel);
	pause_music_label = App->gui->CreateLabel({ 80, 200 }, "fonts/button_text.ttf", 25, "Music", { 255,255,255,255 }, 400, pause_panel);
	pause_music = App->gui->CreateScrollBar({ 100,260 }, { 771,1245,413,40 }, MUSIC, App->audio->GetMusicVolume(), 128, pause_panel);

	pause_fx_label = App->gui->CreateLabel({ 80, 340 }, "fonts/button_text.ttf", 25, "Sound Effect", { 255,255,255,255 }, 400, pause_panel);
	pause_fx = App->gui->CreateScrollBar({100,400 }, { 771,1245,413,40 }, FX, App->audio->GetFxVolume(), 128, pause_panel);

	App->gui->DisableElement(pause_panel);

	//Store 
	if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType() == EntityType::STORE_STRATEGY_BUILDING)
	{
		store_panel = App->gui->CreateImage({ 139,100 }, { 2793, 960, 749, 565 });
		store_unit_one = App->gui->CreateSelectableButton({ 50, 115 }, App->gui->LoadUIButton(random_store_unit[0], "upgrade"), store_panel);
		store_unit_01_cost = App->gui->CreateLabel({ 80, 220 }, "fonts/red_alert.ttf", 40, std::to_string(unit_store_cost), { 255,232,2, 255 }, 710, store_panel);
		store_unit_two = App->gui->CreateSelectableButton({ 325, 115 }, App->gui->LoadUIButton(random_store_unit[1], "upgrade"), store_panel);
		store_unit_02_cost = App->gui->CreateLabel({ 355, 220 }, "fonts/red_alert.ttf", 40, std::to_string(unit_store_cost), { 255,232,2, 255 }, 710, store_panel);
		store_unit_three = App->gui->CreateSelectableButton({ 595, 115 }, App->gui->LoadUIButton(random_store_unit[2], "upgrade"), store_panel);
		store_unit_03_cost = App->gui->CreateLabel({ 625, 220 }, "fonts/red_alert.ttf", 40, std::to_string(unit_store_cost), { 255,232,2, 255 }, 710, store_panel);
		store_unit_four = App->gui->CreateSelectableButton({ 50,300 }, App->gui->LoadUIButton(random_store_unit[3], "upgrade"), store_panel);
		store_unit_04_cost = App->gui->CreateLabel({ 80,405 }, "fonts/red_alert.ttf", 40, std::to_string(unit_store_cost), { 255,232,2, 255 }, 710, store_panel);
		store_unit_five = App->gui->CreateSelectableButton({ 325,300 }, App->gui->LoadUIButton(random_store_unit[4], "upgrade"), store_panel);
		store_unit_05_cost = App->gui->CreateLabel({ 355,405 }, "fonts/red_alert.ttf", 40, std::to_string(unit_store_cost), { 255,232,2, 255 }, 710, store_panel);
		store_unit_six = App->gui->CreateSelectableButton({ 595,300 }, App->gui->LoadUIButton(random_store_unit[5], "upgrade"), store_panel);
		store_unit_06_cost = App->gui->CreateLabel({ 625,405 }, "fonts/red_alert.ttf", 40, std::to_string(unit_store_cost), { 255,232,2, 255 }, 710, store_panel);

		current_gold = App->gui->CreateLabel({ 30,450 }, "fonts/red_alert.ttf", 40, "Your gold: " + std::to_string(App->game_manager->gold), { 255,232,2, 255 }, 710, store_panel);
		total_cost = App->gui->CreateLabel({ 500,450 }, "fonts/red_alert.ttf", 40, "Total cost: " + std::to_string(total_cost_acumulated), { 255,232,2, 255 }, 710, store_panel);

		purchase = App->gui->CreateButtonText({ 263, 505 }, { 20, 0 }, purchase_rect, "CONTINUE", { 255,232,2, 255 }, 20, store_panel);

		App->gui->DisableElement((UIElement*)store_panel);
	}

	//End Game Screen Lose
	lose_panel = App->gui->CreateImage({ 17,120 }, { 3986,1646,605,660 });
	lose_text = App->gui->CreateLabel({ 30,30 }, "fonts/red_alert.ttf", 40, "The enemy troops have defeat yours! Allies have win the battle", { 255,232,2, 255 }, 600, lose_panel);
	lose_continue = App->gui->CreateButtonText({ 170, 560 }, { 30,0 }, button_rect, "CONTINUE", { 200,200,200,255 }, 27, lose_panel);
	lose_image = App->gui->CreateImage({ 170, 250 }, { 2033,136,321,204 }, lose_panel);

	App->gui->DisableElement((UIElement*)lose_panel);
}
