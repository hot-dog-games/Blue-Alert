#pragma once
#include "StaticEntity.h"

class EncounterNode;

class StrategyBuilding :
	public StaticEntity
{
public:
	StrategyBuilding(pugi::xml_node entity_node, fPoint position, Faction faction);
	~StrategyBuilding();

	virtual bool Update(float dt);

	void SetInRange(bool in_range);

	bool in_range = false;
	bool im_current_building = false;
};

