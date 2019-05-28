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
	UIImage* banner = nullptr;
	UIImage* options = nullptr;
	UIImage* troops_background = nullptr;
	UIImage* main_panel = nullptr;
	UILabel* gold = nullptr;
	UILabel* text_menu = nullptr;
	UILabel* musiclabel = nullptr;
	UILabel* fxlabel = nullptr;
	UIButton* settings_button = nullptr;
	UIButtonText* resume_settings_button = nullptr;
	UIButtonText* back_menu_button = nullptr;
	UIButtonText* menu_button = nullptr;
	UIButtonText* backbutton_t_b = nullptr;
	UIButtonText* troops_button = nullptr;
	UIButtonText* buildings_button = nullptr;
	UIScrollBar* music_slider = nullptr;
	UIScrollBar* fx_slider = nullptr;

	//Building Menu
	UIImage* buildings_background = nullptr;
	UIImage* core_image = nullptr;
	UILabel* core_title = nullptr;
	UILabel* core_health = nullptr;
	UILabel* core_energy = nullptr;
	UILabel* core_lvl_up_health_cost = nullptr;
	UILabel* core_lvl_up_energy_cost = nullptr;
	UILabel* core_info = nullptr;
	UIButton* core_lvl_up_health = nullptr;
	UIButton* core_lvl_up_energy = nullptr;

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

	UILabel* buildings_title[3] = { nullptr, nullptr, nullptr };
	
	// Troops Menu

	UIButton* deck_buttons[4] = { nullptr, nullptr, nullptr, nullptr };
	UIButton* collection_buttons_allies[9] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	UIButton* collection_buttons_enemies[9] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	UILabel* troops_title[3] = {nullptr, nullptr, nullptr};
	UIButton* change_side_button = nullptr;
	UILabel* side_label = nullptr;

	//Info Showing
	UIImage* info_image = nullptr;
	UILabel* health_label = nullptr;
	UILabel* attack_label = nullptr;
	UILabel* defense_label = nullptr;
	UILabel* units_label = nullptr;
	UILabel* range_label = nullptr;
	UIBar*	 energy_bar = nullptr;


	float drag_threshhold = 0.2f;
	bool dragable = true;

	iPoint limit_center = {-480, 1117};
	int limit_radius = 700;

	iPoint last_camera_position;
	int volume = 64;

};

#endif

