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
	start_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(0)); //Start node creation
	start_encounter->visited = true;
	map_encounters.push_back(start_encounter);

	EncounterNode* infantry_encounter = new EncounterNode();
	infantry_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(3)); 
	start_encounter->AddChild(infantry_encounter);
	map_encounters.push_back(infantry_encounter);

	EncounterNode* land_encounter = new EncounterNode();
	land_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(1));
	infantry_encounter->AddChild(land_encounter);
	map_encounters.push_back(land_encounter);

	EncounterNode* aerial_encounter_02 = new EncounterNode();
	aerial_encounter_02->LoadEncounterInfo(GetXmlEncounterNodeById(2));
	infantry_encounter->AddChild(aerial_encounter_02);
	map_encounters.push_back(aerial_encounter_02);

	EncounterNode* aerial_encounter = new EncounterNode();
	aerial_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(2)); 
	start_encounter->AddChild(aerial_encounter);
	map_encounters.push_back(aerial_encounter);

	EncounterNode* infantry_encounter_02 = new EncounterNode();
	infantry_encounter_02->LoadEncounterInfo(GetXmlEncounterNodeById(3));
	aerial_encounter->AddChild(infantry_encounter_02);
	map_encounters.push_back(infantry_encounter_02);

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

pugi::xml_node EncounterTree::GetXmlEncounterNodeById(int id)
{
	pugi::xml_node encounter;
	encounter = encounter_tree.find_child_by_attribute("id", std::to_string((int)id).c_str());
	return encounter;
}
