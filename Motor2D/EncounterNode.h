#pragma once
#include <vector>
#include "p2Defs.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2Point.h"

struct Encounter {
	std::string name = ""; // indica el tipo de edificio que es.
	int ai_difficulty = 0;
	int deck[4];
	//BUFF
};


class EncounterNode
{
protected:
	EncounterNode* parent;
	std::vector<EncounterNode*> children;

	//------Encounter-----
	Encounter* encounter = nullptr;

	iPoint position = {0,0};

public:
	EncounterNode();
	EncounterNode(iPoint position);
	~EncounterNode();

	void SetParent(EncounterNode * parent);
	EncounterNode * GetParent() const;

	EncounterNode * AddChild(EncounterNode * child);
	std::vector<EncounterNode*> GetChildren() const;

	void SetPosition(iPoint position);
	iPoint GetPosition();

	void LoadEncounterInfo(pugi::xml_node encounter_node);

	//-----Emcounter Accessors-----
	std::string EncounterNode::GetEncounterName() const;

public:

	bool visited = false;

};