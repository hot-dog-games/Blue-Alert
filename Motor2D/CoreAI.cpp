#include "j1App.h"
#include "EntityManager.h"
#include "DynamicEntity.h"
#include "Render.h"
#include "Deck.h"
#include "CardManager.h"
#include "GameManager.h"
#include "p2Log.h"
#include "CoreAI.h"



CoreAI::CoreAI(pugi::xml_node entity_config, fPoint position, Faction faction, pugi::xml_node stats_node): Core(entity_config,position,faction,stats_node)
{

}


CoreAI::~CoreAI()
{
}

bool CoreAI::Update(float dt)
{
	Core::Update(dt);

	dt_sum += dt;
	if (state == STATIC_IDLE || state == STATIC_ATTACKING)
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
				Think();
				dt_sum = 0;
			}
			break;
		case CoreAI::AIState::ACTING:
			UseCard(selected_card, { (float)lanes[selected_lane].area.x + (float)lanes[selected_lane].area.w*0.5f, position.y + 50 });
			ai_state = AIState::WAITING;
			if (deck->cards[selected_card]->type == EntityType::VIRUS)
			{
				if (!App->game_manager->popups[POPUP_SNIPER_COUNTERS])
					App->game_manager->ShowPopUp(POPUP_SNIPER_COUNTERS);
			}
			else if (deck->cards[selected_card]->type == EntityType::HARRIER)
			{
				if (!App->game_manager->popups[POPUP_AREA_COUNTERS])
					App->game_manager->ShowPopUp(POPUP_AREA_COUNTERS);
			}
			else if (deck->cards[selected_card]->type == EntityType::GI)
			{
				if (!App->game_manager->popups[POPUP_MULTIPLE_COUNTERS])
					App->game_manager->ShowPopUp(POPUP_MULTIPLE_COUNTERS);
			}
		default:
			break;
		}
	}

	return true;
}

void CoreAI::Think()
{
	for (uint i = 0; i < 3; ++i)
	{
		AnalyzeLane(i);
	}

	SelectLane();
	SelectCard();


	if (selected_card > -1)
		ai_state = AIState::ACTING;
	else
		ai_state = AIState::WAITING;
}

void CoreAI::SelectLane()
{
	if (lanes[0].lane_priority >= lanes[1].lane_priority)
	{
		if (lanes[0].lane_priority >= lanes[2].lane_priority)
			selected_lane = 0;
		else
			selected_lane = 2;
	}
	else
	{
		if (lanes[1].lane_priority >= lanes[2].lane_priority)
			selected_lane = 1;
		else
			selected_lane = 2;
	}
}

void CoreAI::AnalyzeLane(uint lane)
{
	float player_damage = 0;
	float enemy_damage = 0;
	float closest_enemy_distance = 1000;
	std::list<Entity*> entities;

	App->entity_manager->GetEntitiesInArea(lanes[lane].area, entities);
	lanes[lane].Reset();

	for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if ((*entity)->faction != faction)
		{
			lanes[lane].enemy_units++;

			enemy_damage += (*entity)->GetDamage();
			float enemy_distance = (*entity)->position.DistanceNoSqrt(position);
			closest_enemy_distance = enemy_distance <= closest_enemy_distance ? enemy_distance : closest_enemy_distance;

			if ((*entity)->IsArmored())
				lanes[lane].enemy_armored++;
			if((*entity)->GetAttackType() == AttackType::AT_BASIC)
				lanes[lane].enemy_basic++;
			else if ((*entity)->GetAttackType() == AttackType::AT_PIERCING)
				lanes[lane].enemy_piercing++;
			else 
				lanes[lane].enemy_aoe++;
		}
		else
		{
			lanes[lane].own_units++;
			player_damage += (*entity)->GetDamage();
		}
	}


	lanes[lane].unit_value = enemy_damage - player_damage;
	lanes[lane].distance_value = LINEAR_INTERPOLATION(closest_enemy_distance, 0, 1000, 1000, 0);
	lanes[lane].lane_priority = lanes[lane].unit_value + lanes[lane].distance_value;
}

void CoreAI::SelectCard()
{
	selected_card = -1;
	AttackType counter = AttackType::AT_BASIC;
	AttackType secondary_counter = AttackType::AT_NONE;

	bool has_counter = false;
	bool has_secondary_counter = false;

	if (lanes[selected_lane].enemy_armored > lanes[selected_lane].enemy_basic)
	{
		if (lanes[selected_lane].enemy_armored > lanes[selected_lane].enemy_piercing)
		{
			counter = AttackType::AT_PIERCING;
			if (lanes[selected_lane].enemy_aoe > lanes[selected_lane].enemy_piercing)
				secondary_counter = AttackType::AT_AOE;
			else
				secondary_counter = AttackType::AT_BASIC;
		}
		else
		{
			if (lanes[selected_lane].enemy_aoe > lanes[selected_lane].enemy_piercing)
				counter = AttackType::AT_PIERCING;
			else
				counter = AttackType::AT_BASIC;
		}
	}
	else
	{
		if (lanes[selected_lane].enemy_basic > lanes[selected_lane].enemy_piercing)
		{
			counter = AttackType::AT_AOE;
			secondary_counter = AT_BASIC;
		}
		else
		{
			if (lanes[selected_lane].enemy_aoe > lanes[selected_lane].enemy_piercing)
				counter = AttackType::AT_PIERCING;
			else
				counter = AttackType::AT_BASIC;
		}

	}

	LOG("piercing: %i, armored: %i, basic: %i, aoe: %i", lanes[selected_lane].enemy_piercing, lanes[selected_lane].enemy_armored, lanes[selected_lane].enemy_basic, lanes[selected_lane].enemy_aoe);
	LOG("SELECTED COUNTER IS: %i  SELECTED SECONDARY COUNTER IS: %i", (int)counter, (int)secondary_counter);

	for (int i = 0; i < deck->GetDeckSize(); i++)
	{
		if (deck->cards[i]->info.attack_type == counter)
		{
			has_counter = true;
			if (CanUseCard(i))
			{
				LOG("COUNTER FOUND");
				selected_card = i;
				break;
			}
		}
	}

	if (!has_counter)
	{
		for (int i = 0; i < deck->GetDeckSize(); i++)
		{
			if (deck->cards[i]->info.attack_type == secondary_counter)
			{
				has_secondary_counter = true;
				if (CanUseCard(i))
				{
					LOG("SECONDARY COUNTER FOUND");
					selected_card = i;
					break;
				}
			}
			else if (!deck->cards[i]->info.armored && secondary_counter == AttackType::AT_NONE)
			{
				has_secondary_counter = true;
				if (CanUseCard(i))
				{
					LOG("SECONDARY COUNTER NON BLINDED FOUND");
					selected_card = i;
					break;
				}
			}
		}
	}

	if (!has_secondary_counter && !has_counter)
	{
		selected_card = rand() % deck->GetDeckSize();
	}
}

bool CoreAI::PostUpdate()
{
	Core::PostUpdate();

	if (debug)
	{
		App->render->DrawQuad(lanes[0].area, 0, 0, 255, 100, true, true);
		App->render->DrawQuad(lanes[1].area, 0, 0, 255, 100, true, true);
		App->render->DrawQuad(lanes[2].area, 0, 0, 255, 100, true, true);
	}

	return true;
}

bool CoreAI::Start()
{
	lanes[0].area = { -150, 0 , 120, 1000 };
	lanes[1].area = { -30, 0 , 120, 1000 };
	lanes[2].area = { 90, 0 , 120, 1000 };

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