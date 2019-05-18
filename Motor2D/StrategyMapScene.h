#ifndef _STRATEGY_MAP_H_
#define _STRATEGY_MAP_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UIButtonText.h"
#include "UIPopUp.h"

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

	void InitializeUI();

	bool IsInsideLimits(int mousemotion_x, int mousemotion_y);

private:
	UIImage* banner;
	UIImage* options;
	UIImage* troops_background;
	UIImage* main_panel;
	UILabel* health;
	UILabel* gold;
	UILabel* energy;
	UILabel* text_menu;
	UIButton* settings_button;
	UIButtonText* menu_button;
	UIButtonText* backbutton_t_b;
	UIButtonText* troops_button;
	UIButtonText* buildings_button;

	//Building Menu
	UIImage* buildings_background;
	UIImage* core_image;

	UIImage* building_infantry_image = nullptr;
	UISelectableButton* building_infantry_button = nullptr;
	UILabel* building_infantry_info = nullptr;

	UIImage* building_aerial_image = nullptr;
	UISelectableButton* building_aerial_button = nullptr;
	UILabel* building_aerial_info = nullptr;

	UIImage* building_land_image = nullptr;
	UISelectableButton* building_land_button = nullptr;
	UILabel* building_land_info = nullptr;

	UIButton* level_up = nullptr;
	UILabel* building_title = nullptr;
	
	// Troops Menu
	UIButton* deck_buttons[4];
	UIButton* collection_buttons_allies[9];
	UILabel* troops_title[3];

	//Info Showing
	UIImage* info_image = nullptr;

	UIPopUp* pop_up = nullptr;

	float drag_threshhold = 0.2f;

	iPoint limit_center = {-480, 1117};
	int limit_radius = 700;

	iPoint last_camera_position;

};

#endif

