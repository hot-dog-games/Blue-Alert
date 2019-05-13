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
#include "UIScrollBar.h"
#include "GUI.h"
#include "Pathfinding.h"
#include "SceneManager.h"
#include "TransitionManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "MainMenuScene.h"
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
	App->audio->PlayMusic("audio/music/9.Destroy-Red Alert2_2.ogg");

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
	App->audio->SetMusicVolume();
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
			App->gui->DisableElement(MenuBackground);
			App->gui->EnableElement(optionsbackground);
			

		}
		if (element == back_options_button) {
			App->gui->EnableElement(MenuBackground);
			App->gui->DisableElement(optionsbackground);
		}
	}
	return true;
}

void MainMenuScene::StartUI() {

	SDL_Rect large_button_rect[3];
	large_button_rect[0] = { 0,533,220,51 };
	large_button_rect[1] = { 0,585,220,51 };
	large_button_rect[2] = { 0,637,220,51 };
	
	SDL_Rect back_options_rect[3];
	back_options_rect[0] = {0,712,46,43};
	back_options_rect[1] = {59,712,46,43};
	back_options_rect[2] = {118,712,46,43};

	MenuBackground = App->gui->CreateImage({ 0,0 }, { 1976,987,1024,768 },nullptr);

	optionsbackground = App->gui->CreateImage({ 0,0 }, { 1976,1832,1024,768 },nullptr);
	
	back_options_button = App->gui->CreateButton({10,720},back_options_rect,optionsbackground);
	volume_slider = App->gui->CreateScrollBar({ 100,100 }, {3592,2335,218,40}, App->audio->volume, 128, optionsbackground);
	App->gui->DisableElement(optionsbackground);

	exitbutton = App->gui->CreateButton({ 785,650 }, large_button_rect,MenuBackground);
	exitlabel = App->gui->CreateLabel({ 20,10 }, "fonts/button_text.ttf", 20, "EXIT GAME", {0,0,0,0},0,exitbutton);
	optionsbutton = App->gui->CreateButton({ 785,440 }, large_button_rect, MenuBackground);
	optionslabel = App->gui->CreateLabel({ 40,10 }, "fonts/button_text.ttf", 20, "OPTIONS", { 0,0,0,0 }, 0, optionsbutton);
	newgamebutton = App->gui->CreateButton({ 785,320 }, large_button_rect, MenuBackground);
	newgamelabel = App->gui->CreateLabel({ 20,10 }, "fonts/button_text.ttf", 20, "NEW GAME", { 0,0,0,0 }, 0, newgamebutton);
	continuebutton = App->gui->CreateButton({ 785,380 }, large_button_rect, MenuBackground);
	continuelabel = App->gui->CreateLabel({ 20,10 }, "fonts/button_text.ttf", 20, "CONTINUE", { 0,0,0,0 }, 0, continuebutton);
}
