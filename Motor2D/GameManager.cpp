#include "CardManager.h"
#include "Deck.h"
#include "EncounterNode.h"
#include "EncounterTree.h"
#include "StrategyBuilding.h"
#include "GameManager.h"



GameManager::GameManager()
{

}


GameManager::~GameManager()
{
}

bool GameManager::Awake(pugi::xml_node &)
{

	return true;
}

bool GameManager::Start()
{
	encounter_tree = new EncounterTree();
	encounter_tree->CreateTree();

	return true;
}

bool GameManager::CleanUp()
{
	return true;
}

bool GameManager::PostUpdate()
{
	encounter_tree->UpdateTree();
	return true;
}

EncounterTree * GameManager::GetEncounterTree()
{
	return encounter_tree;
}
