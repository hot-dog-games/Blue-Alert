#ifndef _ENCOUNTER_NODE_H_
#define _ENCOUNTER_NODE_H_

#include <vector>
#include "p2Defs.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2Point.h"

class UIButton;
class StrategyBuilding;

struct Encounter {
	int type = -1;
	std::vector<int> deck;
	int deck_size = 4;
	std::vector<int> rewards;
	int difficulty = 0;
	//BUFF
};


class EncounterNode
{
protected:
	std::vector<EncounterNode*> parents;
	std::vector<EncounterNode*> children;

	int id = 0;

	//------Encounter-----
	Encounter* encounter = nullptr;

	fPoint position = {0,0};

	UIButton* button;
	SDL_Rect button_rect;
	StrategyBuilding* entity;

public:
	EncounterNode(int id);
	~EncounterNode();

	void SetParent(EncounterNode * parent);
	std::vector<EncounterNode*> GetParents() const;

	EncounterNode * AddChild(EncounterNode * child);
	std::vector<EncounterNode*> GetChildren() const;

	void SetPosition(fPoint position);
	fPoint GetPosition();

	void FillRandomEncounterDeck();
	void FillPredefinedEncounterDeck(pugi::xml_node encounter_node);

	void FillPredefinedRewards(pugi::xml_node rewards_node);
	void FillRandomdRewards();

	void CreateNodeEntity();
	void CreateNode();

	UIButton* GetButton();
	StrategyBuilding* GetEntity();

	int GetID();

	//-----Emcounter Accessors-----
	int GetEncounterType() const;
	void SetEncounterType(int type);

	int GetEncounterDifficulty();
	void SetEncounterDifficulty(int diff);

	void SetEncounterDeckSize(int deck_size);

	std::vector<int> GetEncounterDeck();
	std::vector<int> GetEncounterRewards();

	SDL_Rect node_rect;

public:

	bool visited = false;

};

#endif // !_ENCOUNTER_NODE_H_