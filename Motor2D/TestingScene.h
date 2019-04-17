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

private:
	void CreateDrag(int num, UIElement* element);
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

	UIImage* current_drag;
	int card_num;

	uint max_energy;
	uint current_energy;

	Core* test_core;
	Core* test_enemy_core;
};

#endif
