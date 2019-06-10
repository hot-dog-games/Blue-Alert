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
		if (element == credits_button) {
			App->gui->EnableElement(credits_background);
			App->gui->DisableElement(menu_background);
		}
		if (element == back_credits_button) {
			App->gui->EnableElement(menu_background);
			App->gui->DisableElement(credits_background);
		}
		if (element == newgamebutton) {
			App->game_manager->NewGame();
			App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::MAP, Black);
		}
		if (element == continuebutton)
		{
			App->transition_manager->CreateFadeTransition(2.0f, false, 0, Black, true);
		}

		if (element == team_pages[0]) {
			ShellExecuteA(NULL, "open", "https://github.com/AxelAlavedra", NULL, NULL, SW_SHOWNORMAL);
		}
		else if(element == team_pages[1]) {
			ShellExecuteA(NULL, "open", "https://github.com/AlbertCayuela", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element == team_pages[2]) {
			ShellExecuteA(NULL, "open", "https://github.com/AlexandruC5", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element == team_pages[3]) {
			ShellExecuteA(NULL, "open", "https://github.com/Marcgs96", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element == team_pages[4]) {
			ShellExecuteA(NULL, "open", "https://github.com/LaiaMartinezMotis", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element == team_pages[5]) {
			ShellExecuteA(NULL, "open", "https://github.com/AlexMoralesGarcia", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element == team_pages[6]) {
			ShellExecuteA(NULL, "open", "https://github.com/lakaens", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element == team_pages[7]) {
			ShellExecuteA(NULL, "open", "https://github.com/AlejandroParis", NULL, NULL, SW_SHOWNORMAL);
		}

		if (element == social_media[0]) {
			ShellExecuteA(NULL, "open", "https://twitter.com/hotdoggames1", NULL, NULL, SW_SHOWNORMAL);
		}
		else if(element == social_media[1]) {
			ShellExecuteA(NULL, "open", "https://www.instagram.com/hotdoggames/", NULL, NULL, SW_SHOWNORMAL);
		}
		else if (element == social_media[2]) {
			ShellExecuteA(NULL, "open", "https://github.com/hot-dog-games/Blue-Alert", NULL, NULL, SW_SHOWNORMAL);
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
	music_slider = App->gui->CreateScrollBar({ 340,680 }, { 939,1365,218,40 }, MUSIC, App->audio->GetMusicVolume(), 128, optionsbackground);
	musiclabel = App->gui->CreateLabel({ 80,690 }, "fonts/button_text.ttf", 20, "Music Volume", { 255,255,255,0 }, 0, optionsbackground);
	fx_slider = App->gui->CreateScrollBar({ 340,780 }, { 939,1365,218,40 }, FX, App->audio->GetFxVolume(), 128, optionsbackground);
	fxlabel = App->gui->CreateLabel({ 80,790 }, "fonts/button_text.ttf", 20, "FX Volume", { 255,255,255,0 }, 0, optionsbackground);
	App->gui->DisableElement(optionsbackground);

	exitbutton = App->gui->CreateButtonText({ 180,865 }, { 90,10 }, large_button_rect, "EXIT" ,{ 255,255,255,255 }, 23, menu_background);
	optionsbutton = App->gui->CreateButtonText({ 180,770 }, { 60,10 }, large_button_rect, "OPTIONS", { 255,255,255,255 }, 23, menu_background);
	newgamebutton = App->gui->CreateButtonText({ 180,580 }, { 50,10 }, large_button_rect, "NEW GAME", { 255,255,255,255 }, 23, menu_background);
	continuebutton = App->gui->CreateButtonText({ 180,675 }, { 50,10 }, large_button_rect, "CONTINUE", { 255,255,255,255 }, 23, menu_background, App->HasSave());

	//Credits
	SDL_Rect credits_rect[3];
	credits_rect[0] = { 494,2048,38,38 };
	credits_rect[1] = { 494,2088,38,38 };
	credits_rect[2] = { 494,2129,38,38 };

	SDL_Rect red_button[3];
	red_button[0] = { 12,1861,225,43 };
	red_button[1] = { 12,2101,225,43 };
	red_button[2] = { 12,2341,225,43 };

	SDL_Rect orange_button[3];
	orange_button[0] = { 12,1906,225,43 };
	orange_button[1] = { 12,2146,225,43 };
	orange_button[2] = { 12,2386,225,43 };

	SDL_Rect yellow_green_button[3];
	yellow_green_button[0] = { 12,1951,225,43 };
	yellow_green_button[1] = { 12,2191,225,43 };
	yellow_green_button[2] = { 12,2431,225,43 };

	SDL_Rect blue_light_button[3];
	blue_light_button[0] = { 12,1997,225,43 };
	blue_light_button[1] = { 12,2237,225,43 };
	blue_light_button[2] = { 12,2477,225,43 };

	SDL_Rect purple_light_button[3];
	purple_light_button[0] = { 12,2042,225,43 };
	purple_light_button[1] = { 12,2282,225,43 };
	purple_light_button[2] = { 12,2522,225,43 };

	SDL_Rect pink_button[3];
	pink_button[0] = { 251,1861,225,43 };
	pink_button[1] = { 251,2101,225,43 };
	pink_button[2] = { 251,2341,225,43 };

	SDL_Rect purple_dark_button[3];
	purple_dark_button[0] = { 251,1906,225,43 };
	purple_dark_button[1] = { 251,2146,225,43 };
	purple_dark_button[2] = { 251,2386,225,43 };

	SDL_Rect blue_dark_button[3];
	blue_dark_button[0] = { 251,1951,225,43 };
	blue_dark_button[1] = { 251,2191,225,43 };
	blue_dark_button[2] = { 251,2431,225,43 };

	SDL_Rect twitter_rect[3];
	twitter_rect[0] = { 493,1862,50,41 };
	twitter_rect[1] = { 493,1913,50,41 };
	twitter_rect[2] = { 493,1966,50,41 };

	SDL_Rect instagram_rect[3];
	instagram_rect[0] = { 624,1854,47,47 };
	instagram_rect[1] = { 624,1914,47,47 };
	instagram_rect[2] = { 624,1974,47,47 };

	SDL_Rect github_rect[3];
	github_rect[0] = { 557,1851,52,52 };
	github_rect[1] = { 557,1910,52,52 };
	github_rect[2] = { 557,1968,52,52 };

	credits_button = App->gui->CreateButtonText({ 540, 900 }, { 20,0 }, credits_rect, "C", {255,255,255,255}, 22, menu_background);

	credits_background = App->gui->CreateImage({ 0,0 }, { 2056,1220,640,960 }, nullptr);
	back_credits_button = App->gui->CreateButton({ 520,875 }, back_options_rect, credits_background);
	
	social_media[0] = App->gui->CreateButton({ 345, 800 }, twitter_rect, credits_background);
	social_media[1] = App->gui->CreateButton({ 425, 797 }, instagram_rect, credits_background);
	social_media[2] = App->gui->CreateButton({ 505, 795 }, github_rect, credits_background);

	team_pages[0] = App->gui->CreateButtonText({ 90,590 }, { 20,0 }, red_button, "Axel Alavedra", {0,0,0,0}, 15, credits_background);
	team_pages[1] = App->gui->CreateButtonText({ 90,665 }, { 20,0 }, orange_button, "Albert Cayuela", { 0,0,0,0 }, 14, credits_background);
	team_pages[2] = App->gui->CreateButtonText({ 90,740 }, { 20,0 }, yellow_green_button, "Drum Cercel", { 0,0,0,0 }, 15, credits_background);
	team_pages[3] = App->gui->CreateButtonText({ 90,815 }, { 20,0 }, blue_light_button, "Marc Guillen", { 0,0,0,0 }, 15, credits_background);
	team_pages[4] = App->gui->CreateButtonText({ 90,890 }, { 20,0 }, purple_light_button, "Laia Martinez", { 0,0,0,0 }, 15, credits_background);
	team_pages[5] = App->gui->CreateButtonText({ 340,590 }, { 20,0 }, pink_button, "Alex Morales", { 0,0,0,0 }, 15, credits_background);
	team_pages[6] = App->gui->CreateButtonText({ 340,665 }, { 20,0 }, purple_dark_button, "Eric Navarro", { 0,0,0,0 }, 15, credits_background);
	team_pages[7] = App->gui->CreateButtonText({ 340,740 }, { 20,0 }, blue_dark_button, "Alejandro Paris", { 0,0,0,0 }, 14, credits_background);

	team_label[0] = App->gui->CreateLabel({ 130, 565 }, "fonts/gunplay.ttf", 17, "Code Programmer", { 255,57,57,255 }, 200, credits_background);
	team_label[1] = App->gui->CreateLabel({ 170, 640 }, "fonts/gunplay.ttf", 17, "Manager", { 255,112,57,255 }, 200, credits_background);
	team_label[2] = App->gui->CreateLabel({ 190, 715 }, "fonts/gunplay.ttf", 17, "QA", { 255,199,57,255 }, 200, credits_background);
	team_label[3] = App->gui->CreateLabel({ 185, 790 }, "fonts/gunplay.ttf", 17, "Lead", { 57,57,255,255 }, 200, credits_background);
	team_label[4] = App->gui->CreateLabel({ 180, 865 }, "fonts/gunplay.ttf", 17, "Artist", { 203,72,240,255 }, 200, credits_background);
	team_label[5] = App->gui->CreateLabel({ 420, 565 }, "fonts/gunplay.ttf", 17, "UI Lead", { 255,57,151,255 }, 200, credits_background);
	team_label[6] = App->gui->CreateLabel({ 440, 640 }, "fonts/gunplay.ttf", 17, "UI", { 169,131,180,255 }, 200, credits_background);
	team_label[7] = App->gui->CreateLabel({ 425, 715 }, "fonts/gunplay.ttf", 17, "Design", { 57,166,255,255 }, 200, credits_background);

	App->gui->DisableElement(credits_background);
}
