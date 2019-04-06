#include "j1App.h"
#include "GUI.h"
#include "EntityManager.h"
#include "Entity.h"
#include "EncounterNode.h"


EncounterNode::EncounterNode()
{
	encounter = new Encounter();
	button_rect = { 1000, 1000, 147, 127 };
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
	if (children.size() == 0)
	{
		child->SetPosition({ position.x - rand() % 150 + (-100), position.y - 200 });
	}
	else {
		child->SetPosition({ children.back()->position.x + 300, children.back()->position.y });
	}
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

void EncounterNode::CreateNodeEntity()
{
	entity = App->entity_manager->CreateStrategyBuilding(TESTSTRATEGYBUILDING, { position.x, position.y }, visited? FACTION_RUSSIAN : FACTION_AMERICAN);
}

void EncounterNode::CreateNodeButton()
{
	button = App->gui->CreateButton({ (int)position.x - 72, (int)position.y -127}, &button_rect);
}

void EncounterNode::CreateNode()
{
	CreateNodeEntity();
	CreateNodeButton();
}

UIButton * EncounterNode::GetButton()
{
	return button;
}

StrategyBuilding * EncounterNode::GetEntity()
{
	return entity;
}
