#ifndef _TESTING_SCENE_H_
#define _TESTING_SCENE_H_

#include "Scene.h"
#include "p2Point.h"
#include "EncounterTree.h"

struct SDL_Texture;
struct UIButton;
struct UIImage;
class Entity;
class UIBar;
class Core;
class CoreAI;
class UIButton;
enum CardNumber;

struct Card;

class TestingScene : public Scene
{
public:
	TestingScene();
	~TestingScene();

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
	void CreateDrag(int num, int type, UIElement* element);
	void ReleaseDrag();

private:
	enum class BattleSceneState {
		SETUP,
		FIGHT,
		WIN,
		LOSE
	};
	BattleSceneState state = BattleSceneState::FIGHT;

	SDL_Texture*	debug_tex;

	UIImage*		unit_panel;
	UIButton*		unit_button_one;
	UIButton*		unit_button_two;
	UIButton*		unit_button_three;
	UIButton*		unit_button_four;
	UIBar*			energy_bar;
	UIBar*			health_bar;
	UIImage*		health_bar_image;

	UIImage*		win_panel_one;
	UIImage*		win_panel_two;
	UIButton*		win_continue_one;
	UIButton*		win_continue_two;
	UILabel*		win_text_one;
	UILabel*		win_text_two;
	UIImage*		win_building;
	UIButton*		win_unit_one;
	UIButton*		win_unit_two;
	UIButton*		win_unit_three;

	UIImage*		lose_panel;
	UILabel*		lose_text;
	UIButton*		lose_continue;

	UIImage* current_drag;
	int card_num;
	int random_num[3];

	uint max_energy;
	uint current_energy;

	Core* test_core;
	Core* test_enemy_core;
};

#endif
