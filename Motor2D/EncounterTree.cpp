#include "j1App.h"
#include "GUI.h"
#include "TransitionManager.h"
#include "EntityManager.h"
#include "Render.h"
#include "StrategyBuilding.h"
#include "EncounterTree.h"



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
		iPoint world_position = App->render->ScreenToWorld(node.attribute("x").as_int(),  node.attribute("y").as_int());
		map_encounters[i]->SetPosition({(float)world_position.x, (float)world_position.y});
		map_encounters[i]->SetEncounterName(node.attribute("type").as_string());
		for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling())
		{
			map_encounters[i]->AddChild(map_encounters[child.attribute("id").as_int()]);
		}
	}


	LOG("NODES", map_encounters.size());

	return this;
}

bool EncounterTree::LoadDocuments()
{
	pugi::xml_parse_result result = encounters.load_file("xml/encounters.xml");

	if (result == NULL)
		LOG("Could not load card xml file. pugi error: %s", result.description());
	else
		encounter_tree = encounters.child("encounter_tree");

	result = nodes_01.load_file("xml/map01_nodes.xml");

	if (result == NULL)
		LOG("Could not load card xml file. pugi error: %s", result.description());
	else
		map01_nodes = nodes_01.child("map01_nodes");

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

void EncounterTree::SetCurrentNode(EncounterNode * current_node)
{
	this->current_node = current_node;
	this->current_node->GetEntity()->im_current_building = true;
	this->current_node->visited = true;
}

void EncounterTree::DrawTreeLines()
{
	for each (EncounterNode* n in map_encounters)
	{
		if (n->GetChildren().size() != 0)
		{
			if (n != current_node)
			{
				for (int i = 0; i < n->GetChildren().size(); i++)
				{
					App->render->DrawLine(n->GetPosition().x, n->GetPosition().y, n->GetChildren()[i]->GetPosition().x, n->GetChildren()[i]->GetPosition().y, 255, 0, 0);
				}
			}
			else {
				for (int i = 0; i < n->GetChildren().size(); i++)
				{
					App->render->DrawLine(n->GetPosition().x, n->GetPosition().y, n->GetChildren()[i]->GetPosition().x, n->GetChildren()[i]->GetPosition().y, 0, 255, 0);
				}
			}
		}
	}
}

void EncounterTree::UpdateTreeState()
{
	if(!current_node)SetCurrentNode(map_encounters.front());

	for (int i = 0; i < current_node->GetChildren().size(); i++)
	{
		current_node->GetChildren()[i]->GetEntity()->SetInRange(true);
	}

	current_node->GetEntity()->SetInRange(true);
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

void EncounterTree::UpdateTree()
{

}

void EncounterTree::CleanTree()
{
	for each (EncounterNode* en in map_encounters)
	{
		//if(en->GetEntity() != nullptr)App->entity_manager->DeleteEntity((Entity*)en->GetEntity());
		//App->gui->DeleteElement((UIElement*)en->GetButton());
		delete en;
	}

	delete this;
}
