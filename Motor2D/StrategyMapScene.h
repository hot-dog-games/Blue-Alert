#ifndef _STRATEGY_MAP_H_
#define _STRATEGY_MAP_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UIScrollBar.h"



struct SDL_Texture;

class StrategyMapScene :
	public Scene
{
public:
	StrategyMapScene();
	~StrategyMapScene();

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

	void AddCardToDeck(UIElement* element, uint num);

	void StartUI();

private:
	UIImage* banner;
	UIImage* options;
	UIImage* troops_background;
	UIImage* main_panel;
	UILabel* health;
	UILabel* gold;
	UILabel* energy;
	UILabel* text_menu;
	UILabel* troops_text;
	UILabel* buildings_text;
	UILabel* back_cross_text;
	UILabel* musiclabel;
	UILabel* fxlabel;
	UILabel* savegamelabel;
	UILabel* resume_label;
	UIScrollBar* music_slider;
	UIScrollBar* fx_slider;
	UIButton* settings_button;
	UIButton* resume_settings_button;
	UIButton* menu_button;
	UIButton* backbutton_t_b;
	UIButton* troops_button;
	UIButton* buildings_button;
	UIButton* savegamebutton;
	UIButton* deck_buttons[4];
	UIButton* collection_buttons[9];

};

#endif

