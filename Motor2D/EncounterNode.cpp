#include "j1App.h"
#include "GUI.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Render.h"
#include "Map.h"
#include "UIButton.h"
#include "StrategyBuilding.h"
#include <time.h>
#include "EncounterNode.h"


EncounterNode::EncounterNode(int id)
{
	this->id = id;
	encounter = new Encounter();
	button_rect = { 1000, 1000, 147, 127 };
}

EncounterNode::~EncounterNode()
{
}

int EncounterNode::GetEncounterType() const
{
	return encounter->type;
}

void EncounterNode::SetEncounterType(int type)
{
	encounter->type = type;
}

int EncounterNode::GetEncounterDifficulty()
{
	return encounter->difficulty;
}

void EncounterNode::SetEncounterDifficulty(int diff)
{
	encounter->difficulty = diff;
}

void EncounterNode::SetEncounterDeckSize(int deck_size)
{
	encounter->deck_size = deck_size;
}

std::vector<int> EncounterNode::GetEncounterDeck()
{
	return encounter->deck;
}

std::vector<int> EncounterNode::GetEncounterRewards()
{
	return encounter->rewards;
}

void EncounterNode::SetParent(EncounterNode * parent)
{
	parents.push_back(parent);
}

std::vector<EncounterNode*> EncounterNode::GetParents() const
{
	return parents;
}

EncounterNode * EncounterNode::AddChild(EncounterNode * child)
{
	child->SetParent(this);
	children.push_back(child);
	return child;
}

std::vector<EncounterNode*> EncounterNode::GetChildren() const
{
	return children;
}

void EncounterNode::SetPosition(fPoint position)
{
	this->position = position;
}

fPoint EncounterNode::GetPosition()
{
	return position;
}

void EncounterNode::FillRandomEncounterDeck()
{
	std::vector<int> pool = { 1, 3, 5, 7, 9, 11, 13, 15, 17 };
	std::vector<int>::iterator it;

	for (int i = 0; i < encounter->deck_size; i++)
	{
		int position = rand() % pool.size();
		int card = pool[position];
		encounter->deck.push_back(card);
		it = pool.begin() + position;
		pool.erase(it);
	}
}

void EncounterNode::FillPredefinedEncounterDeck(pugi::xml_node encounter_node)
{
	for (pugi::xml_node card = encounter_node.child("card"); card; card = card.next_sibling("card"))
	{
		encounter->deck.push_back(card.attribute("value").as_int());
	}
}

void EncounterNode::FillPredefinedRewards(pugi::xml_node rewards_node)
{
	for (pugi::xml_node card = rewards_node.child("card"); card; card = card.next_sibling("card"))
	{
		encounter->rewards.push_back(card.attribute("value").as_int());
	}
}

void EncounterNode::FillRandomdRewards()
{
	std::vector<int> pool = { 2, 4, 6, 8, 10, 12, 14, 16, 18 };
	std::vector<int>::iterator it;

	for (int i = 0; i < 3; i++)
	{
		int position = rand() % pool.size();
		int card = pool[position];
		encounter->rewards.push_back(card);
		it = pool.begin() + position;
		pool.erase(it);
	}
}

void EncounterNode::CreateNodeEntity()
{
	iPoint world_position = App->map->MapToWorld(position.x, position.y);
	entity = App->entity_manager->CreateStrategyBuilding((EntityType)encounter->type, { (float)world_position.x, (float)world_position.y }, visited? FACTION_RUSSIAN : FACTION_AMERICAN);
}

void EncounterNode::CreateNode()
{
	CreateNodeEntity();
}

UIButton * EncounterNode::GetButton()
{
	return button;
}

StrategyBuilding * EncounterNode::GetEntity()
{
	return entity;
}

int EncounterNode::GetGoldReward()
{
	return gold_reward;
}

void EncounterNode::SetGoldReward()
{
	if (encounter->type == EntityType::GOLD_STRATEGY_BUILDING)gold_reward = 300;
	else gold_reward = 100;
}

int EncounterNode::GetID()
{
	return id;
}
