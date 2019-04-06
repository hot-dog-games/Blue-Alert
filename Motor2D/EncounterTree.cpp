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
	LoadDocument();

	// 0 - start, 1 - land, 2 - aerial, 3 - infantry

	EncounterNode* start_encounter = new EncounterNode();
	start_encounter->SetPosition({ 500, 700 });
	start_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(0)); //Start node creation
	start_encounter->visited = true;
	map_encounters.push_back(start_encounter);

	EncounterNode* infantry_encounter = new EncounterNode();
	start_encounter->AddChild(infantry_encounter);
	infantry_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(3));
	map_encounters.push_back(infantry_encounter);

	EncounterNode* land_encounter = new EncounterNode();
	infantry_encounter->AddChild(land_encounter);
	land_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(1));
	map_encounters.push_back(land_encounter);

	EncounterNode* aerial_encounter_02 = new EncounterNode();
	infantry_encounter->AddChild(aerial_encounter_02);
	aerial_encounter_02->LoadEncounterInfo(GetXmlEncounterNodeById(2));
	map_encounters.push_back(aerial_encounter_02);

	EncounterNode* aerial_encounter = new EncounterNode();
	start_encounter->AddChild(aerial_encounter);
	aerial_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(2)); 
	map_encounters.push_back(aerial_encounter);


	LOG("NODES", map_encounters.size());

	return this;
}

bool EncounterTree::LoadDocument()
{
	pugi::xml_parse_result result = encounters.load_file("xml/encounters.xml");

	if (result == NULL)
		LOG("Could not load card xml file. pugi error: %s", result.description());
	else
		encounter_tree = encounters.child("encounter_tree");

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
	SetCurrentNode(map_encounters.front());

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
