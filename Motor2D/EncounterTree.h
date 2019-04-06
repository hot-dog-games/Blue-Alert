#pragma once

#include "p2Defs.h"
#include "p2Log.h"
#include "PugiXml\src\pugixml.hpp"
#include "EncounterNode.h"


class EncounterTree
{
private:
	std::vector<EncounterNode*> map_encounters;

	EncounterNode* current_node = nullptr;

	pugi::xml_document encounters;
	pugi::xml_node encounter_tree;

public:
	EncounterTree();
	~EncounterTree();

	EncounterTree* CreateTree();
	bool LoadDocument();

	std::vector<EncounterNode*> GetNodes();

	EncounterNode* GetCurrentNode();
	void SetCurrentNode(EncounterNode* current_node);

	void DrawTreeLines();
	void UpdateTreeState();

	void CreateAllNodes();

	pugi::xml_node GetXmlEncounterNodeById(int id);

	void UpdateTree();

	void CleanTree();

};

