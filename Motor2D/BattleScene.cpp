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
#include "Fonts.h"
#include "GameManager.h"
#include "Particles.h"
#include "EncounterTree.h"
#include "EncounterNode.h"
#include "ProjectileParticle.h"
#include "UIImage.h"
#include "UILabel.h"

const double BOMB_CD = 30;

BattleScene::BattleScene() : Scene()
{

}

// Destructor
BattleScene::~BattleScene()
{}

// Called before the first frame
bool BattleScene::Start()
{
	if (App->map->Load(App->game_manager->GetBattleMap().c_str()) == true)
	{
		int w, h;
		uchar* data = NULL;
		if (App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	App->render->camera.x = (int)(290 * App->win->GetScale());
	App->render->camera.y = (int)(-635 * App->win->GetScale());

	Deck* enemy_deck = new Deck();
	enemy_deck->delete_cards = true;
	for (int i = 0; i < App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDeck().size(); i++)
	{
		enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterDeck()[i]));
	}

	SetEnemiesUpgrades(enemy_deck);

	iPoint allied_core_world = App->map->MapToWorld(48, 48);
	iPoint enemy_core_world = App->map->MapToWorld(25, 25);

	allied_core = App->entity_manager->CreateCore(1, { (float)allied_core_world.x + App->map->data.tile_width*0.5f,  (float)allied_core_world.y + App->map->data.tile_height*0.5f },
		App->game_manager->GetPlayerDeck(), FACTION_RUSSIAN);
	enemy_core = App->entity_manager->CreateCore(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType(),
		{ (float)enemy_core_world.x + App->map->data.tile_width*0.5f,  (float)enemy_core_world.y}, enemy_deck, FACTION_AMERICAN, true);
	enemy_core->delete_deck = true;

	allied_core->SetPivot(PivotType::PVT_CENTER);
	enemy_core->SetPivot(PivotType::PVT_CENTER);

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
	nuke_fx = App->audio->LoadFx("audio/fx/Ambient_Sounds/Shots/nuke_fall.wav");
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
			App->game_manager->gold += App->game_manager->GetEncounterTree()->GetFightingNode()->GetGoldReward();
			App->game_manager->enemy_scaling += 1;

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType() != EntityType::STORE_STRATEGY_BUILDING)
			{
				App->gui->EnableElement((UIElement*)win_panel_one);
				App->game_manager->LevelUpgrade((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType());
			}
			else 
			{
				App->gui->EnableElement((UIElement*)store_panel);
				current_gold->SetText("Your gold: " + std::to_string(App->game_manager->gold) + "g");
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
			App->game_manager->gold += App->game_manager->GetEncounterTree()->GetFightingNode()->GetGoldReward();
			App->game_manager->enemy_scaling += 1;

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType() != EntityType::STORE_STRATEGY_BUILDING)
			{
				App->gui->EnableElement((UIElement*)win_panel_one);
				App->game_manager->LevelUpgrade((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType());
			}
			else
			{
				App->gui->EnableElement((UIElement*)store_panel);
				current_gold->SetText("Your gold: " + std::to_string(App->game_manager->gold) + "g");
			}
		}

		energy_label->SetText(std::to_string(energy_bar->GetValue()));
		if (bomb_cd_timer < BOMB_CD)
		{
			bomb_cd_timer += 1 * dt;
			if (bomb_cd_timer >= BOMB_CD)
			{
				bomb_cd_timer = BOMB_CD;
				bomb_button->SetLocked(true);
			}
		}

		UpdateCooldowns();
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
			if (win_unit[0]->IsSelected()) {
				App->game_manager->AddCardToCollection((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[0]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, Black);
			}
			else if (win_unit[1]->IsSelected()) {
				App->game_manager->AddCardToCollection((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[1]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, Black);
			}
			else if (win_unit[2]->IsSelected()) {
				App->game_manager->AddCardToCollection((EntityType)App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[2]);
				App->gui->DisableElement((UIElement*)win_panel_two);
				App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, Black);
			}
			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetChildren().size() == 0) {
				App->game_manager->change_stage = true;
			}
		}
		else if (element == lose_continue) {
			App->gui->DisableElement((UIElement*)lose_panel);
			App->game_manager->restart = true;
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, Black);
		}

		if (element == win_unit[0]) {
			win_unit[1]->ChangeState(false);
			win_unit[2]->ChangeState(false);
		}
		else if (element == win_unit[1]) {
			win_unit[0]->ChangeState(false);
			win_unit[2]->ChangeState(false);
		}
		else if (element == win_unit[2]) {
			win_unit[1]->ChangeState(false);
			win_unit[0]->ChangeState(false);
		}

		if (element == store_unit[0]) {
			if (store_unit[0]->selected)UpdateGoldOnSelect(random_store_unit[0]);
			else UpdateGoldOnUnSelect(random_store_unit[0]);
		}
		if (element == store_unit[1]) {
			if (store_unit[1]->selected)UpdateGoldOnSelect(random_store_unit[1]);
			else UpdateGoldOnUnSelect(random_store_unit[1]);
		}
		if (element == store_unit[2]) {
			if (store_unit[2]->selected)UpdateGoldOnSelect(random_store_unit[2]);
			else UpdateGoldOnUnSelect(random_store_unit[2]);
		}
		if (element == store_unit[3]) {
			if (store_unit[3]->selected)UpdateGoldOnSelect(random_store_unit[3]);
			else UpdateGoldOnUnSelect(random_store_unit[3]);
		}
		if (element == store_unit[4]) {
			if (store_unit[4]->selected)UpdateGoldOnSelect(random_store_unit[4]);
			else UpdateGoldOnUnSelect(random_store_unit[4]);
		}
		if (element == store_unit[5]) {
			if (store_unit[5]->selected)UpdateGoldOnSelect(random_store_unit[5]);
			else UpdateGoldOnUnSelect(random_store_unit[5]);
		}

		if (element == purchase) {
			for each (EntityType et in store_units_purchased)
			{
				App->game_manager->AddCardToCollection(et);
			}
			App->gui->DisableElement((UIElement*)store_panel);

			if (App->game_manager->GetEncounterTree()->GetFightingNode()->GetChildren().size() == 0)
				App->game_manager->change_stage = true;

			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, Black);
		}

		if (element == bomb_button)
		{
			DropNukes();
			bomb_cd_timer = 0;
			bomb_button->SetLocked(false);
		}
		 
		if (element == faction_button)
		{
			if (!App->IsPaused()) {
				App->PauseGame();
				App->gui->EnableElement(counter_panel);
			}
			else {
				App->ResumeGame();
				App->gui->DisableElement(counter_panel);
			}
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
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MENU, Black);
		}
	}
	else if (gui_event == GUI_Event::LEFT_CLICK_UP) {
		if (element == current_drag) {
			ReleaseDrag();
		}
	}
	else if (gui_event == GUI_Event::MOUSE_OVER)
	{
		std::string str_stat = "";
		for (int num = 0; num < 3; ++num) {
			if (element == win_unit[num]) {
				info_image->SetImage(win_unit[num]->GetAnim()[0]);
				if (App->game_manager->IsInCollection((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]))) {
					str_stat = "Health: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "health"));
					health_label->SetText(str_stat);
					health_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "health")));
					str_stat = "Attack: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "damage"));
					attack_label->SetText(str_stat);
					attack_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "damage")));
					str_stat = "Defense: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "defense"));
					defense_label->SetText(str_stat);
					defense_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "defense")));
					str_stat = "Range: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "range"));
					range_label->SetText(str_stat);
					range_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "range")));
					str_stat = "Units: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "units"));
					units_label->SetText(str_stat);
					units_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "units")));
				}
				else {
					str_stat = "Health: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "health"));
					health_label->SetText(str_stat);
					health_label_upgrade->SetText("");
					str_stat = "Attack: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "damage"));
					attack_label->SetText(str_stat);
					attack_label_upgrade->SetText("");
					str_stat = "Defense: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "defense"));
					defense_label->SetText(str_stat);
					defense_label_upgrade->SetText("");
					str_stat = "Range: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "range"));
					range_label->SetText(str_stat);
					range_label_upgrade->SetText("");
					str_stat = "Units: " + std::to_string(App->game_manager->GetCardStat((EntityType)(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[num]), "units"));
					units_label->SetText(str_stat);
					units_label_upgrade->SetText("");
				}
			}
		}
		for (int num = 0; num < 6; ++num) {
			if (element == store_unit[num]) {
				info_image->SetImage(store_unit[num]->GetAnim()[0]);
				if (App->game_manager->IsInCollection(random_store_unit[num])) {
					str_stat = "Health: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "health"));
					health_label->SetText(str_stat);
					health_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(random_store_unit[num]), "health")));
					str_stat = "Attack: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "damage"));
					attack_label->SetText(str_stat);
					attack_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(random_store_unit[num]), "damage")));
					str_stat = "Defense: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "defense"));
					defense_label->SetText(str_stat);
					defense_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(random_store_unit[num]), "defense")));
					str_stat = "Range: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "range"));
					range_label->SetText(str_stat);
					range_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(random_store_unit[num]), "range")));
					str_stat = "Units: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "units"));
					units_label->SetText(str_stat);
					units_label_upgrade->SetText(" + " + std::to_string(App->game_manager->GetCardUpgrade((EntityType)(random_store_unit[num]), "units")));
				}
				else {
					str_stat = "Health: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "health"));
					health_label->SetText(str_stat);
					health_label_upgrade->SetText("");
					str_stat = "Attack: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "damage"));
					attack_label->SetText(str_stat);
					attack_label_upgrade->SetText("");
					str_stat = "Defense: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "defense"));
					defense_label->SetText(str_stat);
					defense_label_upgrade->SetText("");
					str_stat = "Range: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "range"));
					range_label->SetText(str_stat);
					range_label_upgrade->SetText("");
					str_stat = "Units: " + std::to_string(App->game_manager->GetCardStat((EntityType)(random_store_unit[num]), "units"));
					units_label->SetText(str_stat);
					units_label_upgrade->SetText("");
				}
			}
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
	x -= App->render->scaled_viewport.x;
	y -= App->render->scaled_viewport.y;
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
	else App->audio->PlayFx(no_energy.c_str(), 0);

	App->map->SetDrawable("Spawn", 1);
	App->gui->DeleteElement(current_drag);
	current_drag = nullptr;
}

void BattleScene::GenerateRandomAlliedTroop()
{
	std::vector<int> pool = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
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

	current_gold->SetText("Your gold: " + std::to_string(App->game_manager->gold) + "g");
	total_cost->SetText("Total cost: " + std::to_string(total_cost_acumulated) + "g");

	store_units_purchased.push_back((EntityType)unit);
	if (store_units_purchased.size() > 0)purchase->SetText("PURCHASE");
}

void BattleScene::UpdateGoldOnUnSelect(int unit)
{
	total_cost_acumulated -= 100;
	App->game_manager->gold += 100;

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

	current_gold->SetText("Your gold: " + std::to_string(App->game_manager->gold) + "g");
	total_cost->SetText("Total cost: " + std::to_string(total_cost_acumulated) + "g");

	store_units_purchased.remove((EntityType)unit);

	if (store_units_purchased.size() == 0)purchase->SetText("CONTINUE");
}

void BattleScene::UpdateCooldowns()
{
	float current_value;
	uint max_value;
	uint max_height = 79;
	uint current_height;
	for (int i = 0; i < App->game_manager->GetPlayerDeck()->GetDeckSize(); i++)
	{
		max_value = allied_core->GetCard(i)->info.stats.find("energy_cost")->second->GetValue();
		current_value = allied_core->GetEnergy()->GetValue();
		if (current_value <= max_value)
		{
			if (current_value > 0)
			{
				float percent = (float)current_value * (1 / (float)max_value);
				current_height = percent * (max_height);
			}
			else current_height = 0;
			unit_cooldown[i]->SetHeight(max_height - current_height);
		}
	}

	max_height = 50;
	max_value = BOMB_CD;
	current_value = bomb_cd_timer;
	if (current_value <= max_value)
	{
		if (current_value > 0)
		{
			float percent = (float)current_value * (1 / (float)max_value);
			current_height = percent * (max_height);
		}
		else current_height = 0;
		bomb_cd_image->SetHeight(max_height - current_height);
	}
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
	ProjectileParticle* particle;

	particle = (ProjectileParticle*)App->particles->CreateParticle(ParticleType::NUKE_BOMB, { allied_core->position.x - (allied_core->current_frame.w * 2), 0 },
		{ allied_core->position.x - (allied_core->current_frame.w * 2), allied_core->position.y - allied_core->current_frame.h });
	particle->SetCollisionEffect(ParticleType::NUKE_EXPLOSION, 130, Faction::FACTION_RUSSIAN, 10000.0f);
	App->audio->PlayFx(nuke_fx.c_str(), 0, 4);

	particle = (ProjectileParticle*)App->particles->CreateParticle(ParticleType::NUKE_BOMB, { allied_core->position.x, 0 },
		{ allied_core->position.x, allied_core->position.y - (allied_core->current_frame.h * 1.75f) });
	particle->SetCollisionEffect(ParticleType::NUKE_EXPLOSION, 130, Faction::FACTION_RUSSIAN, 10000.0f);
	App->audio->PlayFx(nuke_fx.c_str(), 0, 4);

	particle = (ProjectileParticle*)App->particles->CreateParticle(ParticleType::NUKE_BOMB, { allied_core->position.x + (allied_core->current_frame.w * 2), 0 },
		{ allied_core->position.x + (allied_core->current_frame.w * 2), allied_core->position.y - allied_core->current_frame.h });
	particle->SetCollisionEffect(ParticleType::NUKE_EXPLOSION, 130, Faction::FACTION_RUSSIAN, 10000.0f);
	App->audio->PlayFx(nuke_fx.c_str(), 0, 4);
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

	unit_panel = App->gui->CreateImage({ 80, 820}, { 1231,186,481,155 });
	if (allied_core->GetCard(CN_FIRST)) {
		unit_button_one = App->gui->CreateButton({ 27, 52 }, App->gui->LoadUIButton(allied_core->GetCard(CN_FIRST)->type, "button"), unit_panel);
		unit_cooldown[0] = App->gui->CreateImage({ 0, 0 }, { 1123, 442, 101, 79 }, unit_button_one);
		energy_cost[0] = App->gui->CreateImage({ -8, 60 }, { 1282,349,25,25 }, unit_button_one);
		energy_cost_label[0] = App->gui->CreateLabel({ 7,2 }, "fonts/gunplay.ttf", 18, std::to_string((int)allied_core->GetCard(CN_FIRST)->info.stats.find("energy_cost")->second->GetValue()), { 255,255,255,255 }, 120, energy_cost[0], false);
	}
	if (allied_core->GetCard(CN_SECOND))
	{
		unit_button_two = App->gui->CreateButton({ 136, 52 }, App->gui->LoadUIButton(allied_core->GetCard(CN_SECOND)->type, "button"), unit_panel);
		unit_cooldown[1] = App->gui->CreateImage({ 0, 0 }, { 1123, 442, 101, 79 }, unit_button_two);
		energy_cost[1] = App->gui->CreateImage({ -8, 60 }, { 1282,349,25,25 }, unit_button_two);
		energy_cost_label[1] = App->gui->CreateLabel({ 7,2 }, "fonts/gunplay.ttf", 18, std::to_string((int)allied_core->GetCard(CN_SECOND)->info.stats.find("energy_cost")->second->GetValue()), { 255,255,255,255 }, 120, energy_cost[1], false);
	}
	if (allied_core->GetCard(CN_THIRD)) {
		unit_button_three = App->gui->CreateButton({ 245, 52 }, App->gui->LoadUIButton(allied_core->GetCard(CN_THIRD)->type, "button"), unit_panel);
		unit_cooldown[2] = App->gui->CreateImage({ 0, 0 }, { 1123, 442, 101, 79 }, unit_button_three);
		energy_cost[2] = App->gui->CreateImage({ -8, 60 }, { 1282,349,25,25 }, unit_button_three);
		energy_cost_label[2] = App->gui->CreateLabel({ 7,2 }, "fonts/gunplay.ttf", 18, std::to_string((int)allied_core->GetCard(CN_THIRD)->info.stats.find("energy_cost")->second->GetValue()), { 255,255,255,255 }, 120, energy_cost[2], false);
	}
	if (allied_core->GetCard(CN_FOURTH)) {
		unit_button_four = App->gui->CreateButton({ 354, 52 }, App->gui->LoadUIButton(allied_core->GetCard(CN_FOURTH)->type, "button"), unit_panel);
		unit_cooldown[3] = App->gui->CreateImage({ 0, 0 }, { 1123, 442, 101, 79 }, unit_button_four);
		energy_cost[3] = App->gui->CreateImage({ -8, 60 }, { 1282,349,25,25 }, unit_button_four);
		energy_cost_label[3] = App->gui->CreateLabel({ 7,2 }, "fonts/gunplay.ttf", 18, std::to_string((int)(allied_core->GetCard(CN_FOURTH)->info.stats.find("energy_cost")->second->GetValue())), { 255,255,255,255 }, 120, energy_cost[3], false);
	}

	energy_bar = App->gui->CreateBar({ 30, 10 }, { 1237,141,446,36 }, allied_core->GetEnergy(), BAR_HORITZONTAL, BAR_DYNAMIC, nullptr, unit_panel);
	energy_image = App->gui->CreateImage({ 8, 10 }, { 1238,345,32,32 }, unit_panel);
	energy_label = App->gui->CreateLabel({ 10,4 }, "fonts/gunplay.ttf", 20, "0", { 255,255,255,255 }, 120, energy_image, false);
	energy_label->SetCentered(true);

	SDL_Rect pause_rect[3];
	pause_rect[0] = { 1168,1371,44,36 };
	pause_rect[1] = { 1215,1371,44,36 };
	pause_rect[2] = { 1262,1371,44,36 };
	pause_button = App->gui->CreateButton({ 25, 910 }, pause_rect, nullptr);

	SDL_Rect bomb_button_rect[4];
	bomb_button_rect[0] = { 1316,1304,58,57 };
	bomb_button_rect[1] = { 1376,1304,58,57 };
	bomb_button_rect[2] = { 1434,1304,58,57 };
	bomb_button_rect[3] = { 1316,1304,58,57 };
	bomb_button = App->gui->CreateButton({ 567, 847 }, bomb_button_rect, nullptr);
	bomb_button->SetLocked(false);
	bomb_cd_image = App->gui->CreateImage({ 0,3 }, { 1143, 551, 58, 50 }, bomb_button);

	SDL_Rect faction_button_rect[3];
	faction_button_rect[0] = { 1346,1366,39,39};
	faction_button_rect[1] = { 1387,1366,39,39 };
	faction_button_rect[2] = { 1427,1366,39,39 };
	faction_button = App->gui->CreateButton({ 575, 912 }, faction_button_rect, nullptr);

	counter_panel = App->gui->CreateImage({ 60, 180 }, { 2761,967,518,479 }, nullptr);
	App->gui->DisableElement(counter_panel);

	health_bar_image = App->gui->CreateImage({ 248, 770 }, { 24,1378,144,16 });
	enemy_health_bar_image = App->gui->CreateImage({ 248, 30 }, { 24,1455,144,16 });
	health_bar = App->gui->CreateBar({ 16,5 }, { 24,1404,127,10 }, allied_core->GetHealth(), BarType::BAR_HORITZONTAL, BAR_DYNAMIC, nullptr, health_bar_image, true);
	enemy_health_bar = App->gui->CreateBar({ 16,6 }, { 24,1404,127,10 }, enemy_core->GetHealth(), BarType::BAR_HORITZONTAL, BAR_DYNAMIC, nullptr, enemy_health_bar_image, true);

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
	win_panel_two = App->gui->CreateImage({ 17,120 }, { 4602,1646,605,660 });
	win_text_one = App->gui->CreateLabel({ 30,30 }, "fonts/button_text.ttf", 23, "Congratulations, you've conquered this zone and unlocked the next building!", { 255,232,2, 255 }, 565, win_panel_one);
	gol_reward_text = App->gui->CreateLabel({ 30, 480 }, "fonts/button_text.ttf", 23, "Gold earned: " + std::to_string(App->game_manager->GetEncounterTree()->GetFightingNode()->GetGoldReward()) + "g", { 234,208,31, 255 }, 565, win_panel_one);
	win_text_two = App->gui->CreateLabel({ 30,30 }, "fonts/button_text.ttf", 23, "Upgrade a troop or choose a new one to add it to your deck", { 255,232,2, 255 }, 565, win_panel_two);
	win_continue_one = App->gui->CreateButtonText({ 170, 560 }, { 30,0}, button_rect, "CONTINUE", { 200,200,200,255 }, 27, win_panel_one);
	win_continue_two = App->gui->CreateButtonText({ 170, 560 }, { 30,0 }, button_rect, "CONTINUE", { 200,200,200,255 }, 27, win_panel_two);

	std::string str[3];
	SDL_Color color[3];

	for (int i = 0; i < 3; ++i) {

		if (App->game_manager->IsInCollection(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[i])) {
			str[i] = "Upgrade";
			color[i] = {238,238,0,255};
		}
		else {
			str[i] = "       New";
			color[i] = { 1,255,31,255 };
		}
	}

	App->gui->CreateLabel({ 90, 195 }, "fonts/button_text.ttf", 20, str[0], color[0], 400, win_panel_two);
	App->gui->CreateLabel({ 240, 195 }, "fonts/button_text.ttf", 20, str[1], color[1], 400, win_panel_two);
	App->gui->CreateLabel({ 390, 195 }, "fonts/button_text.ttf", 20, str[2], color[2], 400, win_panel_two);

	win_unit[0] = App->gui->CreateSelectableButton({ 100, 220 }, App->gui->LoadUIButton(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[0], "upgrade"), win_panel_two);
	win_unit[1] = App->gui->CreateSelectableButton({ 260, 220 }, App->gui->LoadUIButton(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[1], "upgrade"), win_panel_two);
	win_unit[2] = App->gui->CreateSelectableButton({ 420, 220 }, App->gui->LoadUIButton(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterRewards()[2], "upgrade"), win_panel_two);

	win_building = App->gui->CreateImage({ 195,240 }, App->gui->LoadUIImage(App->game_manager->GetEncounterTree()->GetFightingNode()->GetEncounterType(), "end_screen"), win_panel_one);

	App->gui->DisableElement((UIElement*)win_panel_one);
	App->gui->DisableElement((UIElement*)win_panel_two);

		//Info
	info_image = App->gui->CreateImage({ 140, 380 }, { 636,853,106,106 }, win_panel_two);
	health_label = App->gui->CreateLabel({ 260, 380 }, "fonts/button_text.ttf", 18, "Health: -", { 231,216,145,255 }, 175, win_panel_two);
	attack_label = App->gui->CreateLabel({ 260, 400 }, "fonts/button_text.ttf", 18, "Attack: -", { 231,216,145,255 }, 175, win_panel_two);
	defense_label = App->gui->CreateLabel({ 260, 420 }, "fonts/button_text.ttf", 18, "Defense: -", { 231,216,145,255 }, 175, win_panel_two);
	range_label = App->gui->CreateLabel({ 260, 440 }, "fonts/button_text.ttf", 18, "Range: -", { 231,216,145,255 }, 130, win_panel_two);
	units_label = App->gui->CreateLabel({ 260, 460 }, "fonts/button_text.ttf", 18, "Units: -", { 231,216,145,255 }, 115, win_panel_two);
	
	health_label_upgrade = App->gui->CreateLabel({ health_label->GetLocalPos().x + health_label->GetLocalRect().w, 380 }, "fonts/button_text.ttf", 18, "", { 48,169,14,255 }, 300, win_panel_two);
	attack_label_upgrade = App->gui->CreateLabel({ attack_label->GetLocalPos().x + attack_label->GetLocalRect().w, 400 }, "fonts/button_text.ttf", 18, "", { 48,169,14,255 }, 300, win_panel_two);
	defense_label_upgrade = App->gui->CreateLabel({ defense_label->GetLocalPos().x + defense_label->GetLocalRect().w, 420 }, "fonts/button_text.ttf", 18, "", { 48,169,14,255 }, 300, win_panel_two);
	range_label_upgrade = App->gui->CreateLabel({ range_label->GetLocalPos().x + range_label->GetLocalRect().w, 440 }, "fonts/button_text.ttf", 18, "", { 48,169,14,255 }, 300, win_panel_two);
	units_label_upgrade = App->gui->CreateLabel({ units_label->GetLocalPos().x + units_label->GetLocalRect().w, 460 }, "fonts/button_text.ttf", 18, "", { 48,169,14,255 }, 300, win_panel_two);

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

		std::string str_store[6];
		SDL_Color color_store[6];

		for (int i = 0; i < 6; ++i) {

			if (App->game_manager->IsInCollection(random_store_unit[i])) {
				str_store[i] = "Upgrade";
				color_store[i] = { 238,238,0,255 };
			}
			else {
				str_store[i] = "       New";
				color_store[i] = { 1,255,31,255 };
			}
		}

		store_panel = App->gui->CreateImage({ 5,4 }, { 3967, 6, 630, 951 });
		store_unit[0] = App->gui->CreateSelectableButton({ 64,159 }, App->gui->LoadUIButton(random_store_unit[0], "upgrade"), store_panel);
		store_unit_01_cost = App->gui->CreateLabel({ 72, 289 }, "fonts/button_text.ttf", 25, std::to_string(unit_store_cost) + "g", { 255,232,2, 255 }, 710, store_panel);
		store_unit[1] = App->gui->CreateSelectableButton({ 264,159 }, App->gui->LoadUIButton(random_store_unit[1], "upgrade"), store_panel);
		store_unit_02_cost = App->gui->CreateLabel({ 272, 289 }, "fonts/button_text.ttf", 25, std::to_string(unit_store_cost) + "g", { 255,232,2, 255 }, 710, store_panel);
		store_unit[2] = App->gui->CreateSelectableButton({ 466,159 }, App->gui->LoadUIButton(random_store_unit[2], "upgrade"), store_panel);
		store_unit_03_cost = App->gui->CreateLabel({ 474, 289 }, "fonts/button_text.ttf", 25, std::to_string(unit_store_cost) + "g", { 255,232,2, 255 }, 710, store_panel);
		store_unit[3] = App->gui->CreateSelectableButton({ 64,376 }, App->gui->LoadUIButton(random_store_unit[3], "upgrade"), store_panel);
		store_unit_04_cost = App->gui->CreateLabel({ 72,506 }, "fonts/button_text.ttf", 25, std::to_string(unit_store_cost) + "g", { 255,232,2, 255 }, 710, store_panel);
		store_unit[4] = App->gui->CreateSelectableButton({ 264,376 }, App->gui->LoadUIButton(random_store_unit[4], "upgrade"), store_panel);
		store_unit_05_cost = App->gui->CreateLabel({ 272,506 }, "fonts/button_text.ttf", 25, std::to_string(unit_store_cost) + "g", { 255,232,2, 255 }, 710, store_panel);
		store_unit[5] = App->gui->CreateSelectableButton({ 466,376 }, App->gui->LoadUIButton(random_store_unit[5], "upgrade"), store_panel);
		store_unit_06_cost = App->gui->CreateLabel({ 474,506 }, "fonts/button_text.ttf", 25, std::to_string(unit_store_cost) + "g", { 255,232,2, 255 }, 710, store_panel);

		App->gui->CreateLabel({ 50, 113 }, "fonts/button_text.ttf", 20, str_store[0], color_store[0], 400, store_panel);
		App->gui->CreateLabel({ 250, 113 }, "fonts/button_text.ttf", 20, str_store[1], color_store[1], 400, store_panel);
		App->gui->CreateLabel({ 452, 113 }, "fonts/button_text.ttf", 20, str_store[2], color_store[2], 400, store_panel);
		App->gui->CreateLabel({ 50, 338 }, "fonts/button_text.ttf", 20, str_store[3], color_store[3], 400, store_panel);
		App->gui->CreateLabel({ 250, 338 }, "fonts/button_text.ttf", 20, str_store[4], color_store[4], 400, store_panel);
		App->gui->CreateLabel({ 452, 338 }, "fonts/button_text.ttf", 20, str_store[5], color_store[5], 400, store_panel);

		current_gold = App->gui->CreateLabel({ 30,860 }, "fonts/button_text.ttf", 20, "Your gold: " + std::to_string(App->game_manager->gold) + "g", { 255,232,2, 255 }, 710, store_panel);
		total_cost = App->gui->CreateLabel({ 30,900 }, "fonts/button_text.ttf", 20, "Total cost: " + std::to_string(total_cost_acumulated) + "g", { 255,232,2, 255 }, 710, store_panel);

		purchase = App->gui->CreateButtonText({ 350, 870 }, { 25, 0 }, purchase_rect, "CONTINUE", { 255,232,2, 255 }, 20, store_panel);

		//Info
		info_image = App->gui->CreateImage({ 64, 630 }, { 636,853,106,106 }, store_panel);
		health_label = App->gui->CreateLabel({ 220, 630 }, "fonts/button_text.ttf", 25, "Health: -", { 231,216,145,255 }, 235, store_panel);
		attack_label = App->gui->CreateLabel({ 220, 660 }, "fonts/button_text.ttf", 25, "Attack: -", { 231,216,145,255 }, 235, store_panel);
		defense_label = App->gui->CreateLabel({ 220, 690 }, "fonts/button_text.ttf", 25, "Defense: -", { 231,216,145,255 }, 235, store_panel);
		range_label = App->gui->CreateLabel({ 220, 720 }, "fonts/button_text.ttf", 25, "Range: -", { 231,216,145,255 }, 200, store_panel);
		units_label = App->gui->CreateLabel({ 220, 750 }, "fonts/button_text.ttf", 25, "Units: -", { 231,216,145,255 }, 190, store_panel);

		health_label_upgrade = App->gui->CreateLabel({ health_label->GetLocalPos().x + health_label->GetLocalRect().w, 630 }, "fonts/button_text.ttf", 25, "", { 48,169,14,255 }, 300, store_panel);
		attack_label_upgrade = App->gui->CreateLabel({ attack_label->GetLocalPos().x + attack_label->GetLocalRect().w, 660 }, "fonts/button_text.ttf", 25, "", { 48,169,14,255 }, 300, store_panel);
		defense_label_upgrade = App->gui->CreateLabel({ defense_label->GetLocalPos().x + defense_label->GetLocalRect().w, 690 }, "fonts/button_text.ttf", 25, "", { 48,169,14,255 }, 300, store_panel);
		range_label_upgrade = App->gui->CreateLabel({ range_label->GetLocalPos().x + range_label->GetLocalRect().w, 720 }, "fonts/button_text.ttf", 25, "", { 48,169,14,255 }, 300, store_panel);
		units_label_upgrade = App->gui->CreateLabel({ units_label->GetLocalPos().x + units_label->GetLocalRect().w, 750 }, "fonts/button_text.ttf", 25, "", { 48,169,14,255 }, 300, store_panel);

		App->gui->DisableElement((UIElement*)store_panel);
	}
	
	//End Game Screen Lose
	lose_panel = App->gui->CreateImage({ 17,120 }, { 3986,1646,605,660 });
	lose_text = App->gui->CreateLabel({ 30,30 }, "fonts/button_text.ttf", 23, "The enemy troops have defeat yours! Allies have win the battle", { 255,232,2, 255 }, 565, lose_panel);
	lose_continue = App->gui->CreateButtonText({ 170, 560 }, { 30,0 }, button_rect, "CONTINUE", { 200,200,200,255 }, 27, lose_panel);
	lose_image = App->gui->CreateImage({ 170, 250 }, { 2033,136,321,204 }, lose_panel);

	App->gui->DisableElement((UIElement*)lose_panel);
}
