#include "j1App.h"
#include "EntityManager.h"
#include "Deck.h"
#include "CardManager.h"
#include "Stat.h"
#include "p2Log.h"
#include "BuffSourceManager.h"
#include "Particles.h"
#include "Map.h"
#include "Audio.h"
#include "Core.h"

Core::Core(pugi::xml_node entity_config, fPoint position, Faction faction, pugi::xml_node stats_node): StaticEntity(entity_config, position, faction)
{
	for (pugi::xml_node iter = stats_node.child("stat"); iter; iter = iter.next_sibling("stat"))
	{
		std::string stat_name = iter.attribute("stat").as_string();

		//Create the stat
		stats.insert(std::pair<std::string, Stat*>(
			stat_name,
			new Stat(iter.attribute("value").as_int())));
	}
	stats.find("energy")->second->DecreaseStat(20);
	current_animation = &animations.find("idle")->second;

	attack_fx = App->audio->LoadFx("audio/fx/Ambient_Sounds/Shots/One_shoot2.wav");
	App->audio->SetFXVolume(attack_fx.c_str(), 30);
}

Core::~Core()
{
}

bool Core::Update(float dt)
{
	if (energy_timer.ReadMs() >= SECOND_MS) {
		stats.find("energy")->second->IncreaseStat(stats.find("energy_regen")->second->GetValue());
		energy_timer.Start();
	}

	if (state == STATIC_DIE && current_animation->isDone())
	{
		state = STATIC_DESTROYED;
		current_animation = &animations.find("destroyed")->second;
	}

	if (state == STATIC_IDLE)
	{
		if (CheckEnemies())
		{
			state = STATIC_ATTACKING;
		}
	}
	else if (state == STATIC_ATTACKING)
	{
		if(objective->IsAlive())
			Attack();
		else
		{
			objective = nullptr;
			state = STATIC_IDLE;
		}
	}

	if (current_animation)
		current_frame = current_animation->GetCurrentFrame(dt);

	return true;
}

bool Core::CleanUp()
{
	std::map<std::string, Stat*>::iterator item;
	for (item = stats.begin(); item != stats.end(); ++item)
	{
		delete item->second;
		stats.erase(item);
	}
	stats.clear();

	deck->CleanUp();

	if(delete_deck)
		delete deck;

	return true;
}

void Core::LoadUnitSprites(int deck_size)
{
	for (int i = 0; i < deck_size; i++)
	{
		if (deck->cards[i])
		{
			deck->cards[i]->LoadSprite();
		}
	}
}
bool Core::UseCard(int number, fPoint position)
{
	if (CanUseCard(number))
	{
		int group_size = deck->cards[number]->info.stats.find("units")->second->GetValue();
		
		App->entity_manager->CreateGroup(group_size,deck->cards[number]->type, position, deck->cards[number], faction);
		stats.find("energy")->second->DecreaseStat(deck->cards[number]->info.stats.find("energy_cost")->second->GetValue());

		return true;
	}

	return false;
}

bool Core::CanUseCard(int number)
{
	if (!deck->cards[number])
		return false;

	uint energy_cost = deck->cards[number]->info.stats.find("energy_cost")->second->GetValue();

	if (stats.find("energy")->second->GetValue() >= energy_cost)
		return true;
	else 
		return false;
}
void Core::SetDeck(Deck* new_deck)
{
	deck = new_deck;
}

Card* Core::GetCard(int card_num) const
{
	if (deck->cards[card_num]) {
		return deck->cards[card_num];
	}
	return nullptr;
}

void Core::Attack()
{
	if (attack_timer.ReadMs() >= SECOND_MS / stats.find("attack_speed")->second->GetValue())
	{
		float attack = stats.find("damage")->second->GetValue();
		objective->DecreaseLife(attack);
		App->particles->CreateParticle(ParticleType::ATTACK_BASIC_SHOT, { position.x, position.y - current_frame.h * 0.5f },
			{ objective->position.x, objective->position.y - objective->current_frame.h * 0.5f });
		App->audio->PlayFx(attack_fx.c_str(), 0, 1);
		attack_timer.Start();
	}
}

bool Core::CheckEnemies()
{
	Entity* closest_entity = nullptr;
	float distance = 10000.0f;
	App->entity_manager->FindClosestEnemy(position, faction, closest_entity, distance);

	if (closest_entity && distance <= stats.find("range")->second->GetValue()* App->map->data.tile_height)
	{
		objective = closest_entity;
		fPoint objective_direction = { objective->position.x - position.x, objective->position.y - position.y };
		float m = sqrtf(pow(objective_direction.x, 2.0f) + pow(objective_direction.y, 2.0f));
		if (m > 0.0f) {
			objective_direction.x /= m;
			objective_direction.y /= m;
		}
		return true;
	}

	return false;
}

Stat* Core::GetEnergy() const
{	
	return stats.find("energy")->second;
}

Stat * Core::GetHealth() const
{
	return stats.find("health")->second;
}
