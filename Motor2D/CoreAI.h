#ifndef _COREAI_H_
#define _COREAI_H_H

#include "Core.h"

const float THINK_DELAY = 1.0F;

class CoreAI : public Core
{
public:
	CoreAI(pugi::xml_node entity_config, fPoint position, Faction faction, pugi::xml_node stats_node);
	~CoreAI();

	virtual bool Update(float dt);
	virtual bool CleanUp();
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
	bool LosingLane(uint lane);

	float dt_sum = THINK_DELAY;

	SDL_Rect lanes[3];
	int lane = 0;
};

#endif // !_COREAI_H_


