#include "j1App.h"
#include "EntityManager.h"
#include "Deck.h"
#include "CardManager.h"
#include "Stat.h"
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
	stats.find("energy")->second->DecreaseStat(10);
	current_animation = &animations.find("idle")->second;
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

bool Core::UseCard(int number, fPoint position)
{
	if (CanUseCard(number))
	{
		App->entity_manager->CreateEntity(deck->cards[number]->type, position, deck->cards[number], faction);
		stats.find("energy")->second->DecreaseStat(deck->cards[number]->info.stats.find("energy_cost")->second->GetValue());
		return true;
	}

	return false;
}

bool Core::CanUseCard(int number)
{
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

Stat* Core::GetEnergy() const
{	
	return stats.find("energy")->second;
}