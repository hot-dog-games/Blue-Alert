#include "EncounterNode.h"
#include "j1App.h"



EncounterNode::EncounterNode()
{
	encounter = new Encounter();
}

EncounterNode::EncounterNode(iPoint position)
{
	encounter = new Encounter();
	this->position = position;
}


EncounterNode::~EncounterNode()
{
}

std::string EncounterNode::GetEncounterName() const
{
	return encounter->name;
}

void EncounterNode::SetParent(EncounterNode * parent)
{
	this->parent = parent;
}

EncounterNode * EncounterNode::GetParent() const
{
	return parent;
}

EncounterNode * EncounterNode::AddChild(EncounterNode * child)
{
	child->SetParent(this);
	child->SetPosition({ position.x + (rand() % 200 - 100), position.y - 100 });
	children.push_back(child);
	return child;
}

std::vector<EncounterNode*> EncounterNode::GetChildren() const
{
	return children;
}

void EncounterNode::SetPosition(iPoint position)
{
	this->position = position;
}

iPoint EncounterNode::GetPosition()
{
	return position;
}


void EncounterNode::LoadEncounterInfo(pugi::xml_node encounter_node)
{
	encounter->name = encounter_node.attribute("name").as_string();
	encounter->ai_difficulty = encounter_node.attribute("ai_difficulty").as_int();

	pugi::xml_node deck_node = encounter_node.child("deck");

	int i = 0;

	for (pugi::xml_node_iterator it = deck_node.begin(); it != deck_node.end(); ++it)
	{
		encounter->deck[i] = it->attribute("type").as_uint();
		i++;
	}
}
