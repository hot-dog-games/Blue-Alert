#ifndef _BATTLE_SCENE_H_
#define _BATTLE_SCENE_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"

struct SDL_Texture;
struct UIButton;
struct UIImage;
class Entity;
class UIBar;
class Core;
class UIButton;

struct Card;

enum CardNumber {
	CN_UNKNOWN = -1,
	CN_FIRST,
	CN_SECOND,
	CN_THIRD,
	CN_FOURTH
};

class BattleScene : public Scene
{
public:
	BattleScene();
	~BattleScene();

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

private:
	void CreateDrag(int num, int type, UIElement* element);
	void ReleaseDrag();
	void GenerateRandomAlliedTroop();

	void UpdateGoldOnSelect(int unit);
	void UpdateGoldOnUnSelect(int unit);

	void StartUI();

private:
	enum class BattleSceneState {
		SETUP,
		FIGHT,
		WIN,
		LOSE
	};
	BattleSceneState state = BattleSceneState::FIGHT;
	PerfTimer shortcut_timer;

	UIImage*		unit_panel;
	UIButton*		unit_button_one;
	UIButton*		unit_button_two;
	UIButton*		unit_button_three;
	UIButton*		unit_button_four;
	UIBar*			energy_bar;
	UIBar*			health_bar;
	UIImage*		health_bar_image;
	UIBar*			enemy_health_bar;
	UIImage*		enemy_health_bar_image;

	//panels
	UIImage*		win_panel_one;
	UIImage*		win_panel_two;
	UIImage*		store_panel;
	UIImage*		lose_panel;

	//buttons
	UIButton*		win_continue_one;
	UIButton*		win_continue_two;
	UIButton*		lose_continue;
	UIButtonText*		purchase;

	//labels
	UILabel*		win_text_one;
	UILabel*		win_text_two;
	UILabel*		lose_text;
	UIImage*		lose_image;

	//store
	UILabel*		store_text;
	UILabel*		store_unit_01_cost;
	UILabel*		store_unit_02_cost;
	UILabel*		store_unit_03_cost;
	UILabel*		store_unit_04_cost;
	UILabel*		store_unit_05_cost;
	UILabel*		store_unit_06_cost;
	UILabel*		current_gold;
	UILabel*		total_cost;

	//Images
	UIImage*		win_building;

	//selectable buttons
	UISelectableButton*		win_unit_one;
	UISelectableButton*		win_unit_two;
	UISelectableButton*		win_unit_three;

	UISelectableButton*		store_unit_one;
	UISelectableButton*		store_unit_two;
	UISelectableButton*		store_unit_three;
	UISelectableButton*		store_unit_four;
	UISelectableButton*		store_unit_five;
	UISelectableButton*		store_unit_six;

	UIImage* current_drag;
	int card_num;
	std::vector<int> random_store_unit;
	std::list<EntityType> store_units_purchased;

	//Energy Bar
	UILabel* energy_label = nullptr;
	UIImage* energy_image = nullptr;

	//Audio fx
	std::string  win_fx;
	std::string  lose_fx;
	std::string  deployment_fx;
	std::string  no_energy;

	//Pause Menu
	UIButton* pause_button;
	UIImage* pause_panel;
	UIButton* p_continue;
	UIButton* p_exit_menu;

	int unit_store_cost = 100;
	int total_cost_acumulated = 0;

	uint max_energy;
	uint current_energy;

	Core* allied_core;
	Core* enemy_core;
};

#endif
