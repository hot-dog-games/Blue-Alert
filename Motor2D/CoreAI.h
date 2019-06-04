#ifndef _COREAI_H_
#define _COREAI_H_H

#include "Core.h"

const float THINK_DELAY = 3.0F;

struct AILane{
	SDL_Rect area;
	float unit_value = 0.0f;
	float distance_value = 0.0f;
	float lane_priority = 0.0f;

	uint enemy_units = 0;
	uint own_units = 0;
	uint enemy_armored = 0;
	uint enemy_piercing = 0;
	uint enemy_basic = 0;
	uint enemy_aoe = 0;

	void Reset()
	{
		enemy_units = 0;
		own_units = 0;
		enemy_armored = 0;
		enemy_piercing = 0;
		enemy_basic = 0;
		enemy_aoe = 0;
	}
};

class CoreAI : public Core
{
public:
	CoreAI(pugi::xml_node entity_config, fPoint position, Faction faction, pugi::xml_node stats_node);
	~CoreAI();

	virtual bool Update(float dt);
	virtual bool Start();
	virtual bool PostUpdate();

private:
	enum class AIState {
		WAITING,
		THINKING,
		ACTING
	};

	AIState ai_state = AIState::WAITING;

	bool CanPlay();
	void Think();
	void AnalyzeLane(uint lane);
	void SelectCard();
	void SelectLane();

	float dt_sum = THINK_DELAY;

	AILane lanes[3];
	fPoint spawns[3];
	int selected_lane = 0;
	int selected_card = 0;
};

#endif // !_COREAI_H_


