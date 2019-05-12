#ifndef _MAIN_MENU_SCENE_H_
#define _MAIN_MENU_SCENE_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"
#include "UIImage.h"
#include "UIButton.h"
#include "UILabel.h"

struct SDL_Texture;

class UIBar;




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
	UIImage* MenuBackground;
	UIImage* optionsbackground;
	UIButton* exitbutton;
	UILabel* exitlabel;
	UILabel* optionslabel;
	UILabel* newgamelabel;
	UILabel* continuelabel;
	UIButton* optionsbutton;
	UIButton* newgamebutton;
	UIButton* continuebutton;
	UIButton* back_options_button;
	UIScrollBar* volume_slider;
	bool gameclose = false;
};

#endif
