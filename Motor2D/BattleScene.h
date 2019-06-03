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

	void UpdateCooldowns();

	void SetEnemiesUpgrades(Deck* enemy_deck);
	void DropNukes();

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

	UIImage*		unit_panel = nullptr;
	UIButton*		unit_button_one = nullptr;
	UIImage*		unit_cooldown[4];
	UIButton*		unit_button_two = nullptr;
	UIButton*		unit_button_three = nullptr;
	UIButton*		unit_button_four = nullptr;
	UIBar*			energy_bar = nullptr;
	UIBar*			health_bar = nullptr;
	UIImage*		health_bar_image = nullptr;
	UIBar*			enemy_health_bar = nullptr;
	UIImage*		enemy_health_bar_image = nullptr;

	//panels
	UIImage*		win_panel_one = nullptr;
	UIImage*		win_panel_two = nullptr;
	UIImage*		store_panel = nullptr;
	UIImage*		lose_panel = nullptr;

	//buttons
	UIButtonText*		win_continue_one = nullptr;
	UIButtonText*		win_continue_two = nullptr;
	UIButtonText*		lose_continue = nullptr;
	UIButtonText*		purchase = nullptr;

	//labels
	UILabel*		win_text_one = nullptr;
	UILabel*		win_text_two = nullptr;
	UILabel*		lose_text = nullptr;
	UIImage*		lose_image = nullptr;

	//store
	UILabel*		store_text = nullptr;
	UILabel*		store_unit_01_cost = nullptr;
	UILabel*		store_unit_02_cost = nullptr;
	UILabel*		store_unit_03_cost = nullptr;
	UILabel*		store_unit_04_cost = nullptr;
	UILabel*		store_unit_05_cost = nullptr;
	UILabel*		store_unit_06_cost = nullptr;
	UILabel*		current_gold = nullptr;
	UILabel*		total_cost = nullptr;

	//Images
	UIImage*		win_building = nullptr;

	//selectable buttons
	UISelectableButton*		win_unit_one = nullptr;
	UISelectableButton*		win_unit_two = nullptr;
	UISelectableButton*		win_unit_three = nullptr;

	UISelectableButton*		store_unit_one = nullptr;
	UISelectableButton*		store_unit_two = nullptr;
	UISelectableButton*		store_unit_three = nullptr;
	UISelectableButton*		store_unit_four = nullptr;
	UISelectableButton*		store_unit_five = nullptr;
	UISelectableButton*		store_unit_six = nullptr;

	UIImage* current_drag = nullptr;
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
	UIButton* pause_button = nullptr;
	UIButton* bomb_button = nullptr;
	UIButton* faction_button = nullptr;
	UIImage* pause_panel = nullptr;
	UIButton* pause_continue = nullptr;
	UIButton* pause_exit = nullptr;
	UILabel* pause_music_label = nullptr;
	UILabel* pause_fx_label = nullptr;
	UILabel* options_label = nullptr;
	UIScrollBar* pause_music = nullptr;
	UIScrollBar* pause_fx = nullptr;
	UIImage* side_troop_panel = nullptr;
	UIImage* side_troop_panel_left = nullptr;
	UIImage* counters_panel = nullptr;
	UIButton* counters_panel_continue = nullptr;
	UIImage* counters_image = nullptr;

	int unit_store_cost = 100;
	int total_cost_acumulated = 0;

	float lerp_speed = 1 / 60;
	float lerp_percent = 0;
	float bomb_cd_timer = 30;

	uint max_energy;
	uint current_energy;

	Core* allied_core = nullptr;
	Core* enemy_core = nullptr;

	UIImage* energy_cost[4];
	UILabel* energy_cost_label[4];
	UIImage* bomb_cd_image = nullptr;
};

#endif
