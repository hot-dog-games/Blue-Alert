#include <iterator>

#include "j1App.h"
#include "EntityManager.h"
#include "CardManager.h"
#include "Deck.h"
#include "Summoner.h"



Summoner::Summoner()
{
}


Summoner::~Summoner()
{

}

bool Summoner::CleanUp()
{
	deck->CleanUp();
	delete deck;

	return true;
}

void Summoner::UseCard(CardNumber number, fPoint position)
{
	uint energy_cost = deck->cards[number]->info.energy_cost;
	if (current_energy >= energy_cost)
	{
		App->entity_manager->CreateEntity(deck->cards[number]->type, position);
		current_energy -= energy_cost;
	}
}

void Summoner::SetDeck(Deck* new_deck)
{
	deck = new_deck;
}

void Summoner::SetMaxEnergy(uint energy)
{
	max_energy = energy;
	current_energy = max_energy;
}