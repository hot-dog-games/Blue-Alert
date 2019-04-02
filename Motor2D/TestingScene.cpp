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
#include "TransitionManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "TestingScene.h"


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
	ui_background = App->tex->Load("ui/background.png");

	Deck* test_deck = new Deck();
	test_deck->delete_cards = true;
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::G_I));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::SNIPER));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::NAVY_SEAL));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::HARRIER));

	test_core = App->entity_manager->CreateCore(EntityType::CORE, { 0,0 }, test_deck, FACTION_RUSSIAN);

	unit_button_one = App->gui->CreateButton({ 790, 365 }, test_core->GetCard(Core::CardNumber::CN_FIRST)->button.anim);
	unit_button_two = App->gui->CreateButton({ 890, 365 }, test_core->GetCard(Core::CardNumber::CN_SECOND)->button.anim);
	unit_button_three = App->gui->CreateButton({ 790, 445 }, test_core->GetCard(Core::CardNumber::CN_THIRD)->button.anim);
	unit_button_four = App->gui->CreateButton({ 890, 445 }, test_core->GetCard(Core::CardNumber::CN_FOURTH)->button.anim);
	
	energy_bar = App->gui->CreateBar({ 764, 358 }, { 601,0,16,274 }, test_core->GetEnergy());

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
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool TestingScene::Update(float dt)
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 1;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		test_core->DecreaseLife(5);

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		test_core->UseCard(Core::CardNumber::CN_FIRST, {(float)p.x, (float)p.y});

	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) {
		App->transition_manager->CreateFadeTransition(3.0f, false, 0, White);
		App->transition_manager->CreateZoomTransition(3.0f);
		//App->transition_manager->CreateCameraTranslation(3.0f, { App->render->camera.x, App->render->camera.y }, { 0, 0 });
	}
		

	return true;
}

// Called each loop iteration
bool TestingScene::PostUpdate()
{
	bool ret = true;

	App->map->Draw();

	App->render->Blit(ui_background, 755, 0, NULL, 0.0f);

	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const std::vector<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->size(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->at(i).x, path->at(i).y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}


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
	int x, y;
	App->input->GetMousePosition(x, y);

	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {
		Core::CardNumber card_num = Core::CardNumber::CN_UNKNOWN;

		if (element == unit_button_one) {
			card_num = Core::CardNumber::CN_FIRST;
		}
		else if (element == unit_button_two) {
			card_num = Core::CardNumber::CN_SECOND;
		}
		else if (element == unit_button_three) {
			card_num = Core::CardNumber::CN_THIRD;
		}
		else if (element == unit_button_four) {
			card_num = Core::CardNumber::CN_FOURTH;
		}

		if (card_num != Core::CardNumber::CN_UNKNOWN) {
			current_drag = App->gui->CreateImage({ 0,0}, test_core->GetCard(card_num)->button.drag, element);

			current_drag->interactable = true;
			current_drag->dragable = true;
			current_drag->clipping = false;
			current_drag->parent_limit = false;
		}

	}
	else if (gui_event == GUI_Event::LEFT_CLICK_UP) {
		if (element == current_drag) {
			test_core->UseCard(Core::CardNumber::CN_FIRST, { float(x),float(y) });
			App->gui->DeleteElement(current_drag);
			current_drag = nullptr;
		}
		else if (element == unit_button_two) {
			test_core->UseCard(Core::CardNumber::CN_SECOND, { float(x),float(y) });
			App->gui->DeleteElement(current_drag);
			current_drag = nullptr;
		}
		else if (element == unit_button_three) {
			test_core->UseCard(Core::CardNumber::CN_THIRD, { float(x),float(y) });
			App->gui->DeleteElement(current_drag);
			current_drag = nullptr;
		}
		else if (element == unit_button_four) {
			test_core->UseCard(Core::CardNumber::CN_FOURTH, { float(x),float(y) });
			App->gui->DeleteElement(current_drag);
			current_drag = nullptr;
		}
		
	}

	return true;
}
