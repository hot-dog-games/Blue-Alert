#include "p2Log.h"

#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "GameManager.h"
#include "TransitionManager.h"
#include "EncounterTree.h"
#include "EncounterNode.h"

#include "StrategyBuilding.h"




StrategyBuilding::StrategyBuilding(pugi::xml_node entity_node, fPoint position, Faction faction) : StaticEntity(entity_node, position, faction)
{
}


StrategyBuilding::~StrategyBuilding()
{
}

bool StrategyBuilding::Update(float dt)
{
	int mouse_x, mouse_y;

	App->input->GetMousePosition(mouse_x, mouse_y);

	iPoint mouse_world = App->render->ScreenToWorld(mouse_x, mouse_y);

	if (in_range)
	{
		if (mouse_world.x > position.x - current_frame.w / 2 && mouse_world.x < position.x + current_frame.w / 2 && mouse_world.y < position.y && mouse_world.y > position.y - current_frame.h)
		{
			if (!im_current_building)
			{
				state = STATIC_HOVERED;
				current_animation = &animations.find("highlight")->second;
				if (App->input->GetMouseButtonDown(1)) {
					App->transition_manager->CreateFadeTransition(2.0f, true, 3, White);
					App->transition_manager->CreateZoomTransition(2.0f);
					App->transition_manager->CreateCameraTranslation(2.0f, { mouse_world.x, mouse_world.y });
					App->game_manager->GetEncounterTree()->SetCurrentNodeByEntity(this);
					
				}
			}
		}
		else
		{
			state = STATIC_IDLE;
			current_animation = &animations.find("in_range")->second;
		}
	}
	else
	{
		state = STATIC_OFF;
		current_animation = &animations.find("off_range")->second;
	}

	if(current_animation)
		current_frame = current_animation->GetCurrentFrame(dt);

	return true;
}

void StrategyBuilding::SetInRange(bool in_range)
{
	this->in_range = in_range;
}