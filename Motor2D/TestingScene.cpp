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
#include "UIImage.h"


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

	App->render->camera.x = (App->map->data.width*App->map->data.tile_width*0.5)*0.5 - 100;
	App->render->camera.y = 0;

	Deck* test_deck = new Deck();
	test_deck->delete_cards = true;
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::G_I));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::SNIPER));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::NAVY_SEAL));
	test_deck->AddCard(App->card_manager->CreateCard(EntityType::HARRIER));

	test_core = App->entity_manager->CreateCore(EntityType::CORE, { 0,700 }, test_deck, FACTION_RUSSIAN);
	test_enemy_core = App->entity_manager->CreateCore(EntityType::CORE, { 0,200 }, test_deck, FACTION_AMERICAN);

	unit_panel = App->gui->CreateImage({ 755,0 }, { 619,0,269,768 });
	unit_button_one = App->gui->CreateButton({ 35, 365 }, test_core->GetCard(CN_FIRST)->button.anim, unit_panel);
	unit_button_two = App->gui->CreateButton({ 135, 365 }, test_core->GetCard(CN_SECOND)->button.anim, unit_panel);
	unit_button_three = App->gui->CreateButton({ 35, 445 }, test_core->GetCard(CN_THIRD)->button.anim, unit_panel);
	unit_button_four = App->gui->CreateButton({ 135, 445 }, test_core->GetCard(CN_FOURTH)->button.anim, unit_panel);
	
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
		test_core->UseCard(CN_FIRST, {(float)p.x, (float)p.y});

	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		test_enemy_core->UseCard(CN_FIRST, { (float)p.x, (float)p.y });


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

	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const std::vector<iPoint> path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path.size(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path.at(i).x, path.at(i).y);
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

void TestingScene::CreateDrag(int num, UIElement* element)
{
	card_num = num;
	current_drag = App->gui->CreateImage({ 0,0 }, test_core->GetCard(card_num)->button.drag, element);
	current_drag->interactable = true;
	current_drag->dragable = true;
	current_drag->clipping = false;
	current_drag->parent_limit = false;
	current_drag->clicked = true;
}

void TestingScene::ReleaseDrag()
{
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint point = App->render->ScreenToWorld(x, y);
	test_core->UseCard(card_num, { float(point.x),float(point.y) });
	App->gui->DeleteElement(current_drag);
	current_drag = nullptr;
}
