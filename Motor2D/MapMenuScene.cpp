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
#include "Summoner.h"
#include "PathFinding.h"
#include "UIAnimatedImage.h"
#include "UIButton.h"
#include "UIBar.h"
#include "GUI.h"
#include "TransitionManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "MapMenuScene.h"

MapMenuScene::MapMenuScene()
{
}

MapMenuScene::~MapMenuScene()
{
}

bool MapMenuScene::Start()
{
	background = App->tex->Load("ui/Background_Select_Level.png");

	banner = App->gui->CreateImage({ 4,5 }, { 1063,0,1017,83 }, NULL);
	SDL_Rect settings_rect[3];
	settings_rect[0] = { 1063,201,59,51 };
	settings_rect[1] = { 1188,201,58,51 };
	settings_rect[2] = { 1312,201,58,51 };

	SDL_Rect menu_rect[3];
	menu_rect[0] = { 1061,333,220,51 };
	menu_rect[1] = { 1324,333,220,51 };
	menu_rect[2] = { 1616,333,220,51 };

	settings_button = App->gui->CreateButton({ 50,700 }, settings_rect, NULL);
	menu_button = App->gui->CreateButton({ 700,700 }, menu_rect, NULL);

	gold = App->gui->CreateLabel({ 90, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "GOLD", { 0,0,0,0 });
	energy = App->gui->CreateLabel({ 450, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "ENERGY", { 0,0,0,0 });
	health = App->gui->CreateLabel({ 860, 30 }, "ui/Fonts/command_and_conquer___logo_font_by_dexistor371-d6k2yvb.ttf", 20, "HEALTH", { 0,0,0,0 });
	return false;
}

bool MapMenuScene::PreUpdate()
{
	return false;
}

bool MapMenuScene::Update(float dt)
{
	
	return false;
}

bool MapMenuScene::PostUpdate()
{
	
	App->render->Blit(background, 0, 0, NULL, 0);

	return false;
}

bool MapMenuScene::CleanUp()
{
	App->tex->UnLoad(background);
	return false;
}

bool MapMenuScene::GUIEvent(UIElement * element, GUI_Event gui_event)
{
	if (gui_event == GUI_Event::LEFT_CLICK_DOWN) {
		if (element == settings_button) {
			//(options = App->gui->CreateImage();
		}
		/*else if (element == menu_button) {
			options = App->gui->CreateImage({ 450,300 }, {}, NULL);

		}*/
		
	}
	return false;
}
