#include "j1App.h"
#include "CardManager.h"
#include "Deck.h"
#include "EncounterNode.h"
#include "EncounterTree.h"
#include "StrategyBuilding.h"
#include "CardManager.h"
#include "Core.h"
#include "EntityManager.h"
#include "BuffSourceManager.h"
#include "Buff.h"
#include "Stat.h"
#include "Input.h"
#include "Render.h"

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
	CreateStage();
	CreateUpgrades();
	CreatePlayerDeck();
	CreateCoreStats();

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
	AddCardToCollection(EntityType::CONSCRIPT);

	combat_deck = new Deck();
	combat_deck->AddCard(GetCardFromCollection(EntityType::CONSCRIPT));

}

void GameManager::CreateStage()
{
	encounter_tree = new EncounterTree();
	encounter_tree->CreateTree();
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
		ClearUpgrades();
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
	health_upgrade = nullptr;
	energy_upgrade = nullptr;
	infantry_upgrade = nullptr;
	land_upgrade = nullptr;
	aerial_upgrade = nullptr;
}

void GameManager::LevelUpgrade()
{
	EntityType building_type = (EntityType)encounter_tree->GetFightingNode()->GetEncounterType();
	switch (building_type)
	{
	case INFANTRY_STRATEGY_BUILDING:
	if (((LeveledUpgrade*)infantry_upgrade)->LevelUp())
	{
		for each (Card* c in collection)
		{
			if (c->buff_type == INFANTRY_STRATEGY_BUILDING)
			{
				infantry_upgrade->RemoveBuffs(c->info.stats);
				infantry_upgrade->GetBuffs(c->info.stats);
			}
		}
	}
	break;
	case AERIAL_STRATEGY_BUILDING:
		if (((LeveledUpgrade*)aerial_upgrade)->LevelUp())
		{
			for each (Card* c in collection)
			{
				if (c->buff_type == INFANTRY_STRATEGY_BUILDING)
				{
					aerial_upgrade->RemoveBuffs(c->info.stats);
					aerial_upgrade->GetBuffs(c->info.stats);
				}
			}
		}
	break;
	case LAND_STRATEGY_BUILDING:
	if (((LeveledUpgrade*)land_upgrade)->LevelUp())
	{
		for each (Card* c in collection)
		{
			if (c->buff_type == INFANTRY_STRATEGY_BUILDING)
			{
				land_upgrade->RemoveBuffs(c->info.stats);
				land_upgrade->GetBuffs(c->info.stats);
			}
		}
	}
	break;
	case GOLD_STRATEGY_BUILDING:
		break;
	default:
		break;
	}
}

BuffSource* GameManager::GetUpgrade(EntityType unit_type)
{
	switch (unit_type)
	{
	case GI:
		return infantry_upgrade;
		break;
	case CONSCRIPT:
		return infantry_upgrade;
		break;
	case VIRUS:
		return infantry_upgrade;
		break;
	case SNIPER:
		return infantry_upgrade;
		break;
	case GUARDIAN_GI:
		return infantry_upgrade;
		break;
	case FLAK_TROOPER:
		return infantry_upgrade;
		break;
	case GRIZZLY:
		return land_upgrade;
		break;
	case RHINO:
		return land_upgrade;
		break;
	case ROBOT:
		return land_upgrade;
		break;
	case DRONE:
		return land_upgrade;
		break;
	case PRISM:
		return land_upgrade;
		break;
	case TESLA:
		return land_upgrade;
		break;
	case NIGHTHAWK:
		return aerial_upgrade;
		break;
	case SIEGECHOPPER:
		return aerial_upgrade;
		break;
	case HARRIER:
		return aerial_upgrade;
		break;
	case MIG:
		return aerial_upgrade;
		break;
	case BLACK_EAGLE:
		return aerial_upgrade;
		break;
	case SPY_PLANE:
		return aerial_upgrade;
		break;
	default:
		break;
	}
	return nullptr;
}

void GameManager::UpgradeHealth()
{
	if (((LeveledUpgrade*)health_upgrade)->LevelUp())
	{
		((LeveledUpgrade*)health_upgrade)->RemoveBuffs(stats);
		((LeveledUpgrade*)health_upgrade)->GetBuffs(stats);
	}
}

void GameManager::UpgradeEnergy()
{
	if (((LeveledUpgrade*)energy_upgrade)->LevelUp())
	{
		((LeveledUpgrade*)energy_upgrade)->RemoveBuffs(stats);
		((LeveledUpgrade*)energy_upgrade)->GetBuffs(stats);
	}
}

void GameManager::CreateCoreStats()
{
	App->entity_manager->GetCoreStats(&stats);
	((LeveledUpgrade*)health_upgrade)->GetBuffs(stats);
	((LeveledUpgrade*)energy_upgrade)->GetBuffs(stats);
}

bool GameManager::IsInCollection(int card_type)
{
	bool ret = false;

	for each (Card* c in collection)
	{
		if (c->type == card_type)
		{
			ret = true;
			break;
		}
	}

	return ret;
}


