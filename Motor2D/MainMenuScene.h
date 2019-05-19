#ifndef _MAIN_MENU_SCENE_H_
#define _MAIN_MENU_SCENE_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"
#include "UIImage.h"
#include "UILabel.h"

struct SDL_Texture;

class UIBar;
class UIButtonText;

class MainMenuScene : public Scene
{
public:
	MainMenuScene();
	~MainMenuScene();

	// Called before the first frame
	virtual bool Start();

	// Called before all Updates
	virtual bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called before all Updates
	virtual bool PostUpdate();

	// Called before quitting
	virtual bool CleanUp();

	virtual bool GUIEvent(UIElement* element, GUI_Event gui_event);

	void StartUI();
private:
	UIImage* menu_background;
	UIImage* optionsbackground;
	UIButtonText* exitbutton;
	UILabel* musiclabel;
	UILabel* fxlabel;
	UIButtonText* optionsbutton;
	UIButtonText* newgamebutton;
	UIButtonText* continuebutton;
	UIButton* back_options_button;
	UIScrollBar* music_slider;
	UIScrollBar* fx_slider;
	bool gameclose = false;

	int volume = 64;
};

#endif

