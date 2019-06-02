#include "p2Defs.h"
#include "j1App.h"
#include "Audio.h"
#include "GUI.h"
#include "UIButtonText.h"
#include "Pathfinding.h"
#include "SceneManager.h"
#include "TransitionManager.h"
#include "GameManager.h"
#include "MainMenuScene.h"


MainMenuScene::MainMenuScene() : Scene()
{

}

// Destructor
MainMenuScene::~MainMenuScene()
{}

// Called before the first frame
bool MainMenuScene::Start()
{

	//Initialize UI
	StartUI();
	App->audio->PlayMusic("audio/music/menu_music.ogg");

	return true;
}

// Called each loop iteration
bool MainMenuScene::PreUpdate()
{


	return true;
}

// Called each loop iteration
bool MainMenuScene::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool MainMenuScene::PostUpdate()
{
	bool ret = true;
	if (gameclose == true) {
		ret = false;
	}

	return ret;
}

// Called before quitting
bool MainMenuScene::CleanUp()
{
	LOG("Freeing scene");



	return true;
}

bool MainMenuScene::GUIEvent(UIElement * element, GUI_Event gui_event)
{

	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {

		if (element == exitbutton) {
			gameclose = true;
		}
		if (element == optionsbutton) {
			App->gui->DisableElement(menu_background);
			App->gui->EnableElement(optionsbackground);
		}
		if (element == back_options_button) {
			App->gui->EnableElement(menu_background);
			App->gui->DisableElement(optionsbackground);
		}
		if (element == newgamebutton) {
			App->game_manager->NewGame();
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, Black);
		}
		if (element == continuebutton)
		{
			App->transition_manager->CreateFadeTransition(2.0f, false, 0, Black, true);S
		}
	}
	return true;
}

void MainMenuScene::StartUI() {

	SDL_Rect large_button_rect[4];
	large_button_rect[0] = { 800,499,294,67 };
	large_button_rect[1] = { 800,569,294,67 };
	large_button_rect[2] = { 800,639,294,67 };
	large_button_rect[3] = { 800,639,294,67 };

	SDL_Rect back_options_rect[3];
	back_options_rect[0] = { 771,1365,46,44 };
	back_options_rect[1] = { 830,1365,46,44 };
	back_options_rect[2] = { 889,1365,46,44 };

	menu_background = App->gui->CreateImage({ 0,0 }, { 3345,1734,640,960 }, nullptr);

	optionsbackground = App->gui->CreateImage({ 0,0 }, { 4607,9,640,960 }, nullptr);

	options_label = App->gui->CreateLabel({ 170, 600 }, "fonts/button_text.ttf", 40, "Options", { 255,255,255,0 }, 0, optionsbackground);

	back_options_button = App->gui->CreateButton({ 520,875 }, back_options_rect, optionsbackground);
	music_slider = App->gui->CreateScrollBar({ 340,680 }, { 939,1365,218,40 }, MUSIC, volume, 128, optionsbackground);
	musiclabel = App->gui->CreateLabel({ 80,690 }, "fonts/button_text.ttf", 20, "Music Volume", { 255,255,255,0 }, 0, optionsbackground);
	fx_slider = App->gui->CreateScrollBar({ 340,780 }, { 939,1365,218,40 }, FX, volume, 128, optionsbackground);
	fxlabel = App->gui->CreateLabel({ 80,790 }, "fonts/button_text.ttf", 20, "FX Volume", { 255,255,255,0 }, 0, optionsbackground);
	App->gui->DisableElement(optionsbackground);

	exitbutton = App->gui->CreateButtonText({ 180,865 }, { 90,10 }, large_button_rect, "EXIT" ,{ 255,255,255,255 }, 23, menu_background);
	optionsbutton = App->gui->CreateButtonText({ 180,770 }, { 60,10 }, large_button_rect, "OPTIONS", { 255,255,255,255 }, 23, menu_background);
	newgamebutton = App->gui->CreateButtonText({ 180,580 }, { 50,10 }, large_button_rect, "NEW GAME", { 255,255,255,255 }, 23, menu_background);
	continuebutton = App->gui->CreateButtonText({ 180,675 }, { 50,10 }, large_button_rect, "CONTINUE", { 255,255,255,255 }, 23, menu_background, App->HasSave());
}
