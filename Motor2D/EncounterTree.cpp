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
#include "Textures.h"
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

	lines_texture = App->tex->Load("sprites/BuildingNodes/lines.png");
	lines_sprites[0] = { 0, 0, 10, 10 };
	lines_sprites[1] = { 0, 12, 10, 10 };
	lines_sprites[2] = { 0, 25, 10, 10 };

	for (int i = 0; i < map01_nodes.attribute("size").as_int(); i++)
	{
		map_encounters.push_back(new EncounterNode(i));
	}

	for (int i = 0; i < map_encounters.size(); i++)
	{
		pugi::xml_node node = map01_nodes.find_child_by_attribute("id", std::to_string((int)i).c_str());
		map_encounters[i]->SetPosition({ node.attribute("x").as_float(),  node.attribute("y").as_float() });
		map_encounters[i]->SetEncounterType(node.attribute("type").as_int());
		map_encounters[i]->SetGoldReward();
		map_encounters[i]->SetEncounterDifficulty(node.attribute("difficulty").as_int());
		for (pugi::xml_node child = node.child("children").first_child(); child; child = child.next_sibling())
		{
			map_encounters[i]->AddChild(map_encounters[child.attribute("id").as_int()]);
		}

		pugi::xml_node encounter = node.child("encounter");

		if (encounter != NULL)
		{
			map_encounters[i]->FillPredefinedEncounterDeck(encounter);
		}
		else {
			map_encounters[i]->SetEncounterDeckSize(node.attribute("deck_size").as_int());
			map_encounters[i]->FillRandomEncounterDeck();
		}

		pugi::xml_node rewards = node.child("rewards");

		if (rewards != NULL)
		{
			map_encounters[i]->FillPredefinedRewards(rewards);
		}
		else {
			map_encounters[i]->FillRandomdRewards();
		}

	}

	if (!current_node)
		SetCurrentNode(map_encounters.front());

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
	case STAGE_02:
		result = nodes_01.load_file("xml/map02_nodes.xml");

		if (result == NULL)
			LOG("Could not load card xml file. pugi error: %s", result.description());
		else
			map01_nodes = nodes_01.child("map02_nodes");

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

EncounterNode * EncounterTree::GetNodeById(int id)
{
	for each (EncounterNode* n in map_encounters)
	{
		if (n->GetID() == id)
			return n;
	}

	return nullptr;
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
	node_position_offset = { 0, current_node->GetEntity()->current_frame.h / 2 };

	for each (EncounterNode* n in map_encounters)
	{
		DrawTreeLine();			
	}
}

void EncounterTree::UpdateTreeState()
{
	green_dot_positions.clear();
	blue_dot_positions.clear();
	red_dot_positions.clear();

	for (int i = 0; i < current_node->GetChildren().size(); i++)
	{
		current_node->GetChildren()[i]->GetEntity()->SetInRange(true);
	}

	for each (EncounterNode* n in map_encounters)
	{
		if (n->GetChildren().size() != 0)
		{
			if (n != current_node)
			{
				for (int i = 0; i < n->GetChildren().size(); i++)
				{
					if (!n->GetChildren()[i]->visited)
					{
						iPoint parent_world_position = App->map->MapToWorld(n->GetPosition().x, n->GetPosition().y);
						iPoint child_world_position = App->map->MapToWorld(n->GetChildren()[i]->GetPosition().x, n->GetChildren()[i]->GetPosition().y);
						SetDotsPositions(parent_world_position, child_world_position, 0);
					}
				}
				
			}
			else {
				for (int i = 0; i < n->GetChildren().size(); i++)
				{
					iPoint parent_world_position = App->map->MapToWorld(n->GetPosition().x, n->GetPosition().y);
					iPoint child_world_position = App->map->MapToWorld(n->GetChildren()[i]->GetPosition().x, n->GetChildren()[i]->GetPosition().y);
					SetDotsPositions(parent_world_position, child_world_position, 1);
				}
			}
		}

		if (n->visited)
		{
			for (int i = 0; i < n->GetParents().size(); i++)
			{
				if (n->GetParents()[i]->visited)
				{
					iPoint parent_world_position = App->map->MapToWorld(n->GetPosition().x, n->GetPosition().y);
					iPoint child_world_position = App->map->MapToWorld(n->GetParents()[i]->GetPosition().x, n->GetParents()[i]->GetPosition().y);
					SetDotsPositions(parent_world_position, child_world_position, 2);
				}
				else {
					iPoint parent_world_position = App->map->MapToWorld(n->GetPosition().x, n->GetPosition().y);
					iPoint child_world_position = App->map->MapToWorld(n->GetParents()[i]->GetPosition().x, n->GetParents()[i]->GetPosition().y);
					SetDotsPositions(parent_world_position, child_world_position, 0);
				}
			}
		}

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
	LOG("encounter tree cleanup");
	for each (EncounterNode* en in map_encounters)
	{
		delete en;
	}

	map_encounters.clear();
	App->tex->UnLoad(lines_texture);

	delete this;
}

void EncounterTree::EntityClicked(StrategyBuilding * entity)
{
	if (is_clickable) {
		SetFightingNodeByEntity(entity);
		App->gui->DisableUI();
		App->transition_manager->CreateFadeTransition(2.0f, true, SceneType::COMBAT, Black);
		//App->transition_manager->CreateCameraTranslation(2.0f, { (int)entity->position.x, (int)entity->position.y });
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

int EncounterTree::GetBuildingsOfType(EntityType type)
{
	int num = 0;

	for each (EncounterNode* en in map_encounters)
	{
		if (en->GetEncounterType() == type)
			if(en->visited)
				num++;
	}
	return num;
}

void EncounterTree::SetDotsPositions(iPoint origin, iPoint destination, int type)
{
	int distance = origin.DistanceTo(destination);

	for (int i = 0; i < distance; i += 16)
	{
		float percent = i / (float)distance;
		if(type == 0)
			red_dot_positions.push_back({ origin.x + percent * (destination.x - origin.x), origin.y + percent * (destination.y - origin.y) });
		else if(type == 1)
			blue_dot_positions.push_back({ origin.x + percent * (destination.x - origin.x), origin.y + percent * (destination.y - origin.y) });
		else if(type == 2)
			green_dot_positions.push_back({ origin.x + percent * (destination.x - origin.x), origin.y + percent * (destination.y - origin.y) });
	}
}

void EncounterTree::DrawTreeLine()
{
	for each (fPoint ip in green_dot_positions)
	{
		App->render->Blit(lines_texture, ip.x - node_position_offset.x, ip.y - node_position_offset.y, &lines_sprites[2]);
	}	
	
	for each (fPoint ip in blue_dot_positions)
	{
		App->render->Blit(lines_texture, ip.x - node_position_offset.x, ip.y - node_position_offset.y, &lines_sprites[1]);
	}	
	
	for each (fPoint ip in red_dot_positions)
	{
		App->render->Blit(lines_texture, ip.x - node_position_offset.x, ip.y - node_position_offset.y, &lines_sprites[0]);
	}
}
