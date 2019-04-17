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

	App->render->camera.x = (App->map->data.width*App->map->data.tile_width*0.5)*0.5 - 100;
	App->render->camera.y = 0;

	Deck* enemy_deck = new Deck();
	enemy_deck->delete_cards = true;
	enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetEncounterDeck()[0]));
	enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetEncounterDeck()[1]));
	enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetEncounterDeck()[2]));
	enemy_deck->AddCard(App->card_manager->CreateCard((EntityType)App->game_manager->GetEncounterTree()->GetCurrentNode()->GetEncounterDeck()[3]));

	Deck* test_deck = new Deck();
	test_deck->delete_cards = true;
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::G_I));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::SNIPER));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::NAVY_SEAL));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::HARRIER));

	test_core = App->entity_manager->CreateCore(1, { 30,750 }, test_deck, FACTION_RUSSIAN);
	test_enemy_core = App->entity_manager->CreateCore(App->game_manager->GetEncounterTree()->GetCurrentNode()->GetEncounterType(), { 25,85 }, enemy_deck, FACTION_AMERICAN);

	unit_panel = App->gui->CreateImage({ 755,0 }, { 619,0,269,768 });
	unit_button_one = App->gui->CreateButton({ 35, 365 }, test_core->GetCard(CN_FIRST)->button.anim, unit_panel);
	unit_button_two = App->gui->CreateButton({ 135, 365 }, test_core->GetCard(CN_SECOND)->button.anim, unit_panel);
	unit_button_three = App->gui->CreateButton({ 35, 445 }, test_core->GetCard(CN_THIRD)->button.anim, unit_panel);
	unit_button_four = App->gui->CreateButton({ 135, 445 }, test_core->GetCard(CN_FOURTH)->button.anim, unit_panel);

	energy_bar = App->gui->CreateBar({ 764, 358 }, { 601,0,16,274 }, test_core->GetEnergy());

	win_fx = App->audio->LoadFx("audio/fx/Mission/You_victorious.wav");
	lose_fx = App->audio->LoadFx("audio/fx/Mission/You_Lost.wav");
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

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			test_core->DecreaseLife(5);

		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			test_core->UseCard(CN_FIRST, { (float)p.x, (float)p.y });

		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			test_enemy_core->UseCard(CN_FIRST, { (float)p.x, (float)p.y });


		if (!test_core->IsAlive())
		{
			state = BattleSceneState::LOSE;
			App->audio->PlayFx(lose_fx, 0);
			App->PauseGame();
		}
		else if (!test_enemy_core->IsAlive())
		{
			state = BattleSceneState::WIN;
			App->audio->PlayFx(win_fx, 0);
			App->PauseGame();
		}
	}
	break;
	case BattleScene::BattleSceneState::WIN:
	{

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
		
	}
	break;
	case BattleScene::BattleSceneState::LOSE:
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
bool BattleScene::PostUpdate()
{
	bool ret = true;

	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	if (current_drag)
		current_drag->GetScreenPos();

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
			CreateDrag(CN_FIRST, element);
		}
		else if (element == unit_button_two) {
			CreateDrag(CN_SECOND, element);
		}
		else if (element == unit_button_three) {
			CreateDrag(CN_THIRD, element);
		}
		else if (element == unit_button_four) {
			CreateDrag(CN_FOURTH, element);
		}
	}
	else if (gui_event == GUI_Event::LEFT_CLICK_UP) {
		if (element == current_drag) {
			ReleaseDrag();
		}
	}

	return true;
}

void BattleScene::CreateDrag(int num, UIElement* element)
{
	card_num = num;
	current_drag = App->gui->CreateImage({ 0,0 }, test_core->GetCard(card_num)->button.drag, element);
	current_drag->interactable = true;
	current_drag->dragable = true;
	current_drag->clipping = false;
	current_drag->parent_limit = false;
	current_drag->clicked = true;
}

void BattleScene::ReleaseDrag()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint point = App->render->ScreenToWorld(x, y);
	test_core->UseCard(card_num, { float(point.x),float(point.y) });
	App->gui->DeleteElement(current_drag);
	current_drag = nullptr;
}
