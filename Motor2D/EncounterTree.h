#pragma once

#include "p2Defs.h"
#include "p2Log.h"
#include "PugiXml\src\pugixml.hpp"
#include "EncounterNode.h"

class EncounterTree
{
private:
	std::vector<EncounterNode*> map_encounters;

	pugi::xml_document encounters;
	pugi::xml_node encounter_tree;

public:
	EncounterTree();
	~EncounterTree();

	EncounterTree* CreateMap();
	bool LoadDocument();

	pugi::xml_node GetXmlEncounterNodeById(int id);
};

