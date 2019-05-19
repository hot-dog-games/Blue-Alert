#ifndef _ENCOUNTER_TREE_H_
#define _ENCOUNTER_TREE_H_

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

	pugi::xml_document nodes_01;
	pugi::xml_node map01_nodes;

public:
	EncounterTree();
	~EncounterTree();

	EncounterTree* CreateTree();
	bool LoadDocuments();

	std::vector<EncounterNode*> GetNodes();

	EncounterNode* GetCurrentNode();
	void SetCurrentNode(EncounterNode* current_node);

	void DrawTreeLines();
	void UpdateTreeState();

	void CreateAllNodes();

	pugi::xml_node GetXmlEncounterNodeById(int id);

	void UpdateTree();

	void CleanTree();

	void EntityClicked(StrategyBuilding* entity);
	void SetCurrentNodeByEntity(StrategyBuilding* entity);

	void SetDotsPositions(iPoint origin, iPoint destination, int type);
	void DrawTreeLine();

	bool is_clickable = true;

	SDL_Rect lines_sprites[3];
	SDL_Texture* lines_texture;

	std::vector<fPoint>green_dot_positions;
	std::vector<fPoint>blue_dot_positions;
	std::vector<fPoint>red_dot_positions;

};

#endif