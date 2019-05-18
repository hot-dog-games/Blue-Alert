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
	EncounterNode* fighting_node = nullptr;

	pugi::xml_document encounters;
	pugi::xml_node encounter_tree;

	pugi::xml_document nodes_01;
	pugi::xml_node map01_nodes;
	pugi::xml_node tutorial_nodes;

	iPoint node_position_offset;

public:
	EncounterTree();
	~EncounterTree();

	EncounterTree* CreateTree();
	bool LoadDocuments();

	std::vector<EncounterNode*> GetNodes();

	EncounterNode* GetCurrentNode();
	EncounterNode* GetFightingNode();

	void SetCurrentNode(EncounterNode* current_node);
	void SetFightingNode(EncounterNode* fighting_node);

	void DrawTreeLines();
	void UpdateTreeState();

	void CreateAllNodes();


	pugi::xml_node GetXmlEncounterNodeById(int id);

	void CleanTree();

	void EntityClicked(StrategyBuilding* entity);
	void SetCurrentNodeByEntity(StrategyBuilding* entity);
	void SetFightingNodeByEntity(StrategyBuilding* entity);
	int GetBuildingsOfType(EntityType type);

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