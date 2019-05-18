#include "j1App.h"
#include "GUI.h"
#include "TransitionManager.h"
#include "EntityManager.h"
#include "SceneManager.h"
#include "Render.h"
#include "Map.h"
#include "StrategyBuilding.h"
#include "EncounterTree.h"
#include "GameManager.h"
#include "Deck.h"



EncounterTree::EncounterTree()
{

}


EncounterTree::~EncounterTree()
{

}

EncounterTree * EncounterTree::CreateTree()
{
	LoadDocuments();

	for (int i = 0; i < map01_nodes.attribute("size").as_int(); i++)
	{
		map_encounters.push_back(new EncounterNode(i));
	}

	for (int i = 0; i < map_encounters.size(); i++)
	{
		pugi::xml_node node = map01_nodes.find_child_by_attribute("id", std::to_string((int)i).c_str());
		map_encounters[i]->SetPosition({ node.attribute("x").as_float(),  node.attribute("y").as_float() });
		map_encounters[i]->SetEncounterType(node.attribute("type").as_int());
		for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
		{
			map_encounters[i]->AddChild(map_encounters[child.attribute("id").as_int()]);
		}
		map_encounters[i]->FillEncounterDeck();
	}

	if (!current_node)
		SetCurrentNode(map_encounters.front());

	LOG("NODES", map_encounters.size());

	return this;
}

bool EncounterTree::LoadDocuments()
{
	pugi::xml_parse_result result;

	switch (App->game_manager->stage)
	{
	case STAGE_TUTORIAL:
		result = nodes_01.load_file("xml/tutorial_nodes.xml");

		if (result == NULL)
			LOG("Could not load card xml file. pugi error: %s", result.description());
		else
			map01_nodes = nodes_01.child("tutorial_nodes");
		break;
	case STAGE_01: 
		result = nodes_01.load_file("xml/map01_nodes.xml");

		if (result == NULL)
			LOG("Could not load card xml file. pugi error: %s", result.description());
		else
			map01_nodes = nodes_01.child("map01_nodes");
		break;
	default:
		break;
	}

	return true;
}

std::vector<EncounterNode*> EncounterTree::GetNodes()
{
	return map_encounters;
}

EncounterNode * EncounterTree::GetCurrentNode()
{
	return current_node;
}

EncounterNode * EncounterTree::GetFightingNode()
{
	return fighting_node;
}

void EncounterTree::SetCurrentNode(EncounterNode * current_node)
{
	this->current_node = current_node;
	this->current_node->visited = true;
}

void EncounterTree::SetFightingNode(EncounterNode * fighting_node)
{
	this->fighting_node = fighting_node;
}

void EncounterTree::DrawTreeLines()
{
	node_position_offset = { current_node->GetEntity()->current_frame.w / 2, current_node->GetEntity()->current_frame.h / 2 };
	for each (EncounterNode* n in map_encounters)
	{
		if (n->GetChildren().size() != 0)
		{
			if (n != current_node)
			{
				for (int i = 0; i < n->GetChildren().size(); i++)
				{
					iPoint parent_world_position = App->map->MapToWorld(n->GetPosition().x, n->GetPosition().y);
					iPoint child_world_position = App->map->MapToWorld(n->GetChildren()[i]->GetPosition().x, n->GetChildren()[i]->GetPosition().y);
					App->render->DrawLine(parent_world_position.x, parent_world_position.y - node_position_offset.y,
						child_world_position.x, child_world_position.y - node_position_offset.y, 255, 0, 0);
				}
			}
			else {
				for (int i = 0; i < n->GetChildren().size(); i++)
				{
					iPoint parent_world_position = App->map->MapToWorld(n->GetPosition().x, n->GetPosition().y);
					iPoint child_world_position = App->map->MapToWorld(n->GetChildren()[i]->GetPosition().x, n->GetChildren()[i]->GetPosition().y);
					App->render->DrawLine(parent_world_position.x, parent_world_position.y - node_position_offset.y,
						child_world_position.x, child_world_position.y - node_position_offset.y, 0, 255, 0);
				}
			}
		}
	}
}

void EncounterTree::UpdateTreeState()
{
	for (int i = 0; i < current_node->GetChildren().size(); i++)
	{
		current_node->GetChildren()[i]->GetEntity()->SetInRange(true);
	}
}

void EncounterTree::CreateAllNodes()
{
	for each (EncounterNode* en in map_encounters)
	{
		en->CreateNode();
	}
}


pugi::xml_node EncounterTree::GetXmlEncounterNodeById(int id)
{
	pugi::xml_node encounter;
	encounter = encounter_tree.find_child_by_attribute("id", std::to_string((int)id).c_str());
	return encounter;
}

void EncounterTree::CleanTree()
{
	for each (EncounterNode* en in map_encounters)
	{
		delete en;
	}

	map_encounters.clear();

	delete this;
}

void EncounterTree::EntityClicked(StrategyBuilding * entity)
{
	if (is_clickable) {
		SetFightingNodeByEntity(entity);
		App->gui->DisableUI();
		App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::COMBAT, White);
		App->transition_manager->CreateZoomTransition(2.0f);
		App->transition_manager->CreateCameraTranslation(2.0f, { (int)entity->position.x, (int)entity->position.y });
	}
}

void EncounterTree::SetCurrentNodeByEntity(StrategyBuilding * entity)
{
	for each (EncounterNode* en in map_encounters)
	{
		if (en->GetEntity() == entity) SetCurrentNode(en);
	}
}

void EncounterTree::SetFightingNodeByEntity(StrategyBuilding * entity)
{
	for each (EncounterNode* en in map_encounters)
	{
		if (en->GetEntity() == entity) SetFightingNode(en);
	}
}
