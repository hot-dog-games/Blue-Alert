#include "p2Log.h"

#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"

#include "StrategyBuilding.h"




StrategyBuilding::StrategyBuilding(pugi::xml_node entity_node, fPoint position, Faction faction) : StaticEntity(entity_node, position, faction)
{
}


StrategyBuilding::~StrategyBuilding()
{
}

bool StrategyBuilding::Update(float dt)
{
	StaticEntity::Update(dt);

	int mouse_x, mouse_y;

	App->input->GetMousePosition(mouse_x, mouse_y);

	if (in_range)
	{
		if (mouse_x > position.x - current_frame.w / 2 && mouse_x < position.x + current_frame.w / 2 && mouse_y < position.y && mouse_y > position.y - current_frame.h)
		{
			state = STATIC_HOVERED;
		}
		else state = STATIC_IDLE;
	}
	else state = STATIC_OFF;

	return true;
}

void StrategyBuilding::SetInRange(bool in_range)
{
	this->in_range = in_range;
}
