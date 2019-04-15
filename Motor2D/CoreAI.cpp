#include "j1App.h"
#include "Deck.h"
#include "p2Log.h"
#include "CoreAI.h"



CoreAI::CoreAI(pugi::xml_node entity_config, fPoint position, Faction faction): Core(entity_config,position,faction)
{

}


CoreAI::~CoreAI()
{
}

bool CoreAI::Update(float dt)
{
	Core::Update(dt);

	dt_sum += dt;
	if (state == STATIC_IDLE)
	{

		switch (ai_state)
		{
		case CoreAI::AIState::WAITING:
			if (CanPlay())
				ai_state = AIState::THINKING;
			break;
		case CoreAI::AIState::THINKING:
			if (dt_sum >= THINK_DELAY)
			{
				for (int i = 0; i < MAX_CARDS; i++)
				{
					if (CanUseCard(i))
					{
						UseCard(i, { position.x, position.y + 50 });
						ai_state = AIState::THINKING;
						break;
					}
				}
				dt_sum = 0;
			}
			break;
		case CoreAI::AIState::ACTING:
			break;
		default:
			break;
		}
	}

	return true;
}

bool CoreAI::CleanUp()
{

	return true;
}

bool CoreAI::CanPlay()
{
	for (int i = 0; i < MAX_CARDS; i++)
	{
		if (CanUseCard(i))
			return true;
	}
	return false;
}
