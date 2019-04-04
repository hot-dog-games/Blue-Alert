#include "j1App.h"
#include "GUI.h"
#include "TransitionManager.h"
#include "EntityManager.h"
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
		if(en->GetEntity() != nullptr)App->entity_manager->DeleteEntity((Entity*)en->GetEntity());
		App->gui->DeleteElement((UIElement*)en->GetButton());
		delete en;
	}

	delete this;
}
