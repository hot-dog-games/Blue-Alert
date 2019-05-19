#include "p2Defs.h"
#include "j1App.h"
#include "Audio.h"
#include "GUI.h"
#include "UIButtonText.h"
#include "Pathfinding.h"
#include "SceneManager.h"
#include "TransitionManager.h"
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
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, White);
		}
	}
	return true;
}

void MainMenuScene::StartUI() {

	SDL_Rect large_button_rect[4];
	large_button_rect[0] = { 0,533,220,51 };
	large_button_rect[1] = { 0,585,220,51 };
	large_button_rect[2] = { 0,637,220,51 };
	large_button_rect[3] = { 221,690,220,51 };

	SDL_Rect back_options_rect[3];
	back_options_rect[0] = { 2794,912,46,44 };
	back_options_rect[1] = { 2853,912,46,44 };
	back_options_rect[2] = { 2912,912,46,44 };

	menu_background = App->gui->CreateImage({ 0,0 }, { 3712,1329,1024,768 }, nullptr);

	optionsbackground = App->gui->CreateImage({ 0,0 }, { 3712,2100,1024,768 }, nullptr);

	back_options_button = App->gui->CreateButton({ 100,500 }, back_options_rect, optionsbackground);
	music_slider = App->gui->CreateScrollBar({ 400,150 }, { 2962,912,218,40 }, MUSIC, volume, 128, optionsbackground);
	musiclabel = App->gui->CreateLabel({ 100,160 }, "fonts/button_text.ttf", 20, "Music Volume", { 255,255,255,0 }, 0, optionsbackground);
	fx_slider = App->gui->CreateScrollBar({ 400,250 }, { 2962,912,218,40 }, FX, volume, 128, optionsbackground);
	fxlabel = App->gui->CreateLabel({ 100,260 }, "fonts/button_text.ttf", 20, "FX Volume", { 255,255,255,0 }, 0, optionsbackground);
	App->gui->DisableElement(optionsbackground);

	exitbutton = App->gui->CreateButtonText({ 785,650 }, { 20,10 }, large_button_rect, "EXIT" ,{ 0,0,0,0 }, 20, menu_background);
	optionsbutton = App->gui->CreateButtonText({ 785,440 }, { 20,10 }, large_button_rect, "OPTIONS", { 0,0,0,0 }, 20, menu_background);
	newgamebutton = App->gui->CreateButtonText({ 785,320 }, { 20,10 }, large_button_rect, "NEW GAME", { 0,0,0,0 }, 20, menu_background);
	continuebutton = App->gui->CreateButtonText({ 785,380 }, { 20,10 }, large_button_rect, "CONTINUE", { 75,8,8,255 }, 20, menu_background, false);
}
