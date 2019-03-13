#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "p2Log.h"
#include "CardManager.h"



CardManager::CardManager()
{
	name = "card_manager";
}


CardManager::~CardManager()
{
}

bool CardManager::CleanUp()
{
	for (std::list<Card*>::iterator card = cards.begin(); card != cards.end(); ++card)
	{
		card = cards.erase(card);
	}
	return true;
}

bool CardManager::PostUpdate()
{
	if (to_delete)
	{
		for (std::list<Card*>::iterator card = cards.begin(); card != cards.end(); ++card)
		{
			if ((*card)->to_delete)
				card = cards.erase(card);
		}

		to_delete = false;
	}
	return true;
}

bool CardManager::Awake(pugi::xml_node& conf)
{
	pugi::xml_parse_result result = config_file.load_file("xml/cards.xml");

	if (result == NULL)
		LOG("Could not card xml file. pugi error: %s", result.description());
	else
		card_configs = config_file.child("config");

	return true;
}

bool CardManager::Start()
{
	CreateCard(G_I);
	CreateCard(G_I);
	test_card = CreateCard(G_I);
	return true;
}

Card* CardManager::CreateCard(EntityType type)
{
	Card* card = new Card;
	card->type = type;
	card->level = 0;

	//TODO look for info in xml with position same as type.
	pugi::xml_node card_node = card_configs.find_child_by_attribute("type", std::to_string((int)type).c_str());
	card->name = card_node.child("name").child_value();

	cards.push_back(card);

	return card;
}

Card* CardManager::DeleteCard(Card* card)
{
	card->to_delete = true;
	to_delete = true;

	return nullptr;
}
