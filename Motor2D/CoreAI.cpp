#include "j1App.h"
#include "EntityManager.h"
#include "Render.h"
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
				for (uint i = 0; i < 3; ++i)
				{
					if (LosingLane(i))
					{
						ai_state = AIState::ACTING;
						lane = i;
						dt_sum = 0;
						break;
					}
				}
			}
			break;
		case CoreAI::AIState::ACTING:
			for (int i = 0; i < MAX_CARDS; i++)
			{
				if (CanUseCard(i))
				{
					LOG("spawned in lane %i", lane);
					UseCard(i, { (float)lanes[lane].x + (float)lanes[lane].w*0.5f, position.y + 50 });
					ai_state = AIState::WAITING;
					break;
				}
			}
			break;
		default:
			break;
		}
	}

	return true;
}

bool CoreAI::PostUpdate()
{
	App->render->DrawQuad(lanes[0], 0, 0, 255, 150, true, true);
	App->render->DrawQuad(lanes[1], 0, 0, 255, 150, true, true);
	App->render->DrawQuad(lanes[2], 0, 0, 255, 150, true, true);

	return true;
}

bool CoreAI::CleanUp()
{

	return true;
}

bool CoreAI::Start()
{
	lanes[0] = { -173, 0 , 135, 1000 };
	lanes[1] = { -37, 0 , 135, 1000 };
	lanes[2] = { 99, 0 , 135, 1000 };

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

bool CoreAI::LosingLane(uint lane)
{
	uint own_units = 0;
	uint enemy_units = 0;
	std::list<Entity*> entities;
	App->entity_manager->GetEntitiesInArea(lanes[lane], entities);

	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if ((*entity)->faction != faction)
			enemy_units++;
		else
			own_units++;
	}

	return (enemy_units >= own_units);	
}
