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
#include "GUI.h"
#include "Window.h"
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
	CreatePopUps();
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
	if (stage == STAGE_TUTORIAL)
	{
		combat_deck = new Deck();
		AddCardToCollection(EntityType::CONSCRIPT);
	}
	else if (stage == STAGE_01)
	{
		combat_deck = new Deck();
		AddCardToCollection(EntityType::CONSCRIPT);
		AddCardToCollection(EntityType::SNIPER);
		AddCardToCollection(EntityType::MIG);
		AddCardToCollection(EntityType::RHINO);
	}
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
		ResetBuildingBuffs();
		ClearUpgrades();
		Start();
		restart = false;
	}

	return true;
}

void GameManager::ResetBuildingBuffs()
{
	for each (Card* c in collection)
	{
		infantry_upgrade->RemoveBuffs(c->info.stats);
		land_upgrade->RemoveBuffs(c->info.stats);
		aerial_upgrade->RemoveBuffs(c->info.stats);
	}

	((LeveledUpgrade*)infantry_upgrade)->Reset();
	((LeveledUpgrade*)land_upgrade)->Reset();
	((LeveledUpgrade*)aerial_upgrade)->Reset();

	for each (Card* c in collection)
	{
		GetUpgrade(c->type)->GetBuffs(c->info.stats);
	}
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
		combat_deck->AddCard(new_card);
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
	health_upgrade->RemoveBuffs(stats);
	energy_upgrade->RemoveBuffs(stats);
	(((LeveledUpgrade*)infantry_upgrade)->Reset());
	(((LeveledUpgrade*)infantry_upgrade)->Reset());
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
				if (c->buff_type == AERIAL_STRATEGY_BUILDING)
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
			if (c->buff_type == LAND_STRATEGY_BUILDING)
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

void GameManager::CreatePopUps()
{
	for (int i = 0 ; i < POPUP_MAX; i++)
	{
		popups[i] = false;
	}
}

void GameManager::ShowPopUp(int popup)
{
	uint mouse_x, mouse_y, screen_width, screen_height;
	App->win->GetWindowSize(screen_width, screen_height);

	switch (popup)
	{
	case POPUP_BUILDING_NODES: 
		App->gui->CreatePopUp({ (int)screen_width / 2, (int)screen_height / 3, 300, 90 }, { 10 , 10 }, 
			"This is an enemy building, you must click on it to begin a fight! Click the button down below to continue", 20, {255,255,255,255});
		break;
	case POPUP_USETROOP: 
		App->gui->CreatePopUp({ ((int)screen_width / 2) - 200, (int)screen_height / 3, 300, 120 }, { 10 , 10 }, 
			"To deploy a troop you need to drag the button of the specific troop in your troops bar. Try dragging your conscript troop into the battlefield!", 20, { 255,255,255,255 });
		break;	
	case POPUP_SNIPER_COUNTERS: 
		App->gui->CreatePopUp({ ((int)screen_width / 2) - 200, (int)screen_height / 3, 300, 120 }, { 10 , 10 },
			"The enemy is about to deploy snipers, your conscripts are a good choice to counter them because they attack slow and you have number of units advantadge!", 20, { 255,255,255,255 });
		break;
	case POPUP_AREA_COUNTERS:
		App->gui->CreatePopUp({ ((int)screen_width / 2) - 200, (int)screen_height / 3, 300, 120 }, { 10 , 10 },
			"The enemy is about to deploy Harriers, your conscripts are a bad choice, try with your new troop because it has a lot of range and piercing ammo!", 20, { 255,255,255,255 });
		break;
	case POPUP_MULTIPLE_COUNTERS:
		App->gui->CreatePopUp({ ((int)screen_width / 2) - 200, (int)screen_height / 3, 300, 120 }, { 10 , 10 },
			"The enemy is about to deploy GI's, use your new troop to destroy them!", 20, { 255,255,255,255 });
		break;
	case POPUP_DECISIONMAKING:
		App->gui->CreatePopUp({ ((int)screen_width / 2) - 150, (int)screen_height / 2, 300, 150 }, { 10 , 10 },
			"It's time to choose your path, after conquering a specific building you'll get the building buff. Check the menu for more info about building buffs. And check your new unlocked troop in the troops menu!", 20, { 255,255,255,255 });
		break;
	case POPUP_STORE:
		App->gui->CreatePopUp({ ((int)screen_width / 2) - 150, (int)screen_height / 2, 300, 150 }, { 10 , 10 },
			"Welcome to the Store! Here you can buy troops from the enemy force that have rebelled to the enemy forces. You buy troops by paying gold you earn 100g per combat.", 20, { 255,255,255,255 });
		break;
	default:
		break;
	}

	popups[popup] = true;
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


