#include "j1App.h"
#include "EntityManager.h"
#include "Deck.h"
#include "CardManager.h"
#include "Stat.h"
#include "Core.h"



Core::Core(pugi::xml_node entity_config, fPoint position): StaticEntity(entity_config, position)
{
	for (pugi::xml_node iter = entity_config.child("stats").child("stat"); iter; iter = iter.next_sibling("stat"))
	{
		std::string stat_name = iter.attribute("stat").as_string();

		//Create the stat
		stats.insert(std::pair<std::string, Stat*>(
			stat_name,
			new Stat(iter.attribute("value").as_int())));
	}

	energy = stats.find("energy")->second;
}

Core::~Core()
{
}

bool Core::CleanUp()
{
	deck->CleanUp();
	delete deck;

	return true;
}

void Core::UseCard(CardNumber number, fPoint position)
{
	uint energy_cost = deck->cards[(int)number]->info.stats.find("energy_cost")->second->GetValue();
	if (energy->GetValue() >= energy_cost)
	{
		App->entity_manager->CreateEntity(deck->cards[(int)number]->type, position, deck->cards[(int)number]);
		energy->DecreaseStat(energy_cost);
	}
}

void Core::SetDeck(Deck* new_deck)
{
	deck = new_deck;
}

Card * Core::GetCard(CardNumber card_num) const
{
	if (deck->cards[card_num]) {
		return deck->cards[card_num];
	}
	return nullptr;
}

Stat * Core::GetEnergy() const
{	
	return stats.find("energy")->second;
}
