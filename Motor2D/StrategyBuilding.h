#pragma once
#include "StaticEntity.h"

class StrategyBuilding :
	public StaticEntity
{
public:
	StrategyBuilding(pugi::xml_node entity_node, fPoint position, Faction faction);
	~StrategyBuilding();

	virtual bool Update(float dt);

};

