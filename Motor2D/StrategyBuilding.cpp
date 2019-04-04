#include "p2Log.h"

#include "j1App.h"
#include "Render.h"
#include "Textures.h"

#include "StrategyBuilding.h"




StrategyBuilding::StrategyBuilding(pugi::xml_node entity_node, fPoint position, Faction faction) : StaticEntity(entity_node, position, faction)
{
	LOG("HOLAEXISTO");
}


StrategyBuilding::~StrategyBuilding()
{
}

bool StrategyBuilding::Update(float dt)
{
	StaticEntity::Update(dt);
	return true;
}
