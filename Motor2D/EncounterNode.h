#pragma once
#include <vector>
#include "p2Defs.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2Point.h"

class UIButton;
class StrategyBuilding;

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

	fPoint position = {0,0};

	UIButton* button;
	SDL_Rect button_rect;
	StrategyBuilding* entity;

public:
	EncounterNode();
	~EncounterNode();

	void SetParent(EncounterNode * parent);
	EncounterNode * GetParent() const;

	EncounterNode * AddChild(EncounterNode * child);
	std::vector<EncounterNode*> GetChildren() const;

	void SetPosition(fPoint position);
	fPoint GetPosition();

	void LoadEncounterInfo(pugi::xml_node encounter_node);
	void CreateNodeEntity();
	void CreateNodeButton();

	//-----Emcounter Accessors-----
	std::string EncounterNode::GetEncounterName() const;

	SDL_Rect node_rect;

public:

	bool visited = false;

};