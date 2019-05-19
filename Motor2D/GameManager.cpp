#include "j1App.h"
#include "CardManager.h"
#include "Deck.h"
#include "EncounterNode.h"
#include "EncounterTree.h"
#include "StrategyBuilding.h"
#include "CardManager.h"
#include "EntityManager.h"

#include "GameManager.h"



GameManager::GameManager()
{

}


GameManager::~GameManager()
{
}

bool GameManager::Awake(pugi::xml_node &)
{

	return true;
}

bool GameManager::Start()
{
	encounter_tree = new EncounterTree();
	encounter_tree->CreateTree();

	CreatePlayerDeck();

	return true;
}

bool GameManager::CleanUp()
{
	delete combat_deck;
	collection.clear();
	encounter_tree->CleanTree();

	return true;
}

EncounterTree* GameManager::GetEncounterTree()
{
	return encounter_tree;
}

Deck * GameManager::GetPlayerDeck()
{
	return combat_deck;
}

bool GameManager::IsInPlayerDeck(Card * card)
{
	bool ret = false;

	Card* deck_card = combat_deck->GetCard(card->type);

	if (deck_card) {
		ret = true;
	}
	else {
		ret = false;
	}

	return ret;
}

void GameManager::CreatePlayerDeck()
{
	collection.push_back(App->card_manager->CreateCard(EntityType::CONSCRIPT));
	collection.push_back(App->card_manager->CreateCard(EntityType::SNIPER));
	collection.push_back(App->card_manager->CreateCard(EntityType::FLAK_TROOPER));
	collection.push_back(App->card_manager->CreateCard(EntityType::GRIZZLY));

	combat_deck = new Deck();
	combat_deck->AddCard(GetCardFromCollection(EntityType::CONSCRIPT));
	combat_deck->AddCard(GetCardFromCollection(EntityType::SNIPER));
	combat_deck->AddCard(GetCardFromCollection(EntityType::FLAK_TROOPER));
	combat_deck->AddCard(GetCardFromCollection(EntityType::GRIZZLY));
}

bool GameManager::Restart()
{
	if (restart)
	{
		for (std::list<Card*>::iterator card = collection.begin(); card != collection.end(); ++card)
		{
			App->card_manager->DeleteCard((*card));
		}
		collection.clear();

		delete combat_deck;
		encounter_tree->CleanTree();
		Start();
		restart = false;
	}

	return true;
}



Card * GameManager::GetCardFromCollection(EntityType card_type)
{
	Card* card = nullptr;
	for each (Card* c in collection)
	{
		if (c->type == card_type)
		{
			card = c;
		}
	}

	if (card != nullptr)
	{
		return card;
	}
	else {
		LOG("The card u tried to get is not existent in collection");
		return nullptr;
	}
}

void GameManager::AddCardToCollection(EntityType card_type)
{
	bool found = false;

	for each (Card* c in collection)
	{
		if (c->type == card_type)
		{
			found = true;
			c->Upgrade();
		}
	}

	if (!found)
	{
		Card* new_card = App->card_manager->CreateCard(card_type);
		collection.push_back(new_card);
		GetUpgrade(card_type)->GetBuffs(new_card->info.stats);
	}
}

void GameManager::CreateUpgrades()
{
	health_upgrade = App->buff->CreateBuffSource("core_health_upgrade");
	energy_upgrade = App->buff->CreateBuffSource("core_energy_upgrade");
	infantry_upgrade = App->buff->CreateBuffSource("troop_buff");
	land_upgrade = App->buff->CreateBuffSource("troop_buff");
	aerial_upgrade = App->buff->CreateBuffSource("troop_buff");
}
void GameManager::ClearUpgrades()
{
	for each (Card* c in collection)
	{
		infantry_upgrade->RemoveBuffs(c->info.stats);
		land_upgrade->RemoveBuffs(c->info.stats);
		aerial_upgrade->RemoveBuffs(c->info.stats);
	}
	health_upgrade->RemoveBuffs(stats);
	energy_upgrade->RemoveBuffs(stats);

	App->buff->CleanUp();
	health_upgrade = nullptr;
	energy_upgrade = nullptr;
	infantry_upgrade = nullptr;
	land_upgrade = nullptr;
	aerial_upgrade = nullptr;
}
