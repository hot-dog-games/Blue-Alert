#include "EncounterTree.h"



EncounterTree::EncounterTree()
{

}


EncounterTree::~EncounterTree()
{

}

EncounterTree * EncounterTree::CreateMap()
{
	LoadDocument();

	// 0 - start, 1 - land, 2 - aerial, 3 - infantry

	EncounterNode* start_encounter = new EncounterNode();
	start_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(0)); //Start node creation
	map_encounters.push_back(start_encounter);

	EncounterNode* infantry_encounter = new EncounterNode();
	infantry_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(3)); 
	map_encounters.push_back(infantry_encounter);
	start_encounter->AddChild(infantry_encounter);

	EncounterNode* aerial_encounter = new EncounterNode();
	aerial_encounter->LoadEncounterInfo(GetXmlEncounterNodeById(2)); 
	map_encounters.push_back(aerial_encounter);
	start_encounter->AddChild(aerial_encounter);

	return this;
}

bool EncounterTree::LoadDocument()
{
	pugi::xml_parse_result result = encounters.load_file("xml/encounters.xml");

	if (result == NULL)
		LOG("Could not load card xml file. pugi error: %s", result.description());
	else
		encounter_tree = encounters.child("config");

	return true;
}

pugi::xml_node EncounterTree::GetXmlEncounterNodeById(int id)
{
	pugi::xml_node encounter;
	encounter = encounter_tree.find_child_by_attribute("id", std::to_string((int)id).c_str());
	return encounter;
}
