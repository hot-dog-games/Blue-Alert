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

void EncounterNode::FillEncounterDeck()
{
	std::vector<int> pool = { 1, 2, 3 ,4 ,5, 6, 7, 8, 9 };
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
