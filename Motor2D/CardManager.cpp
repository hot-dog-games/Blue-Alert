#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "p2Log.h"
#include "Stat.h"
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
	LOG("card manager cleanup");
	while (!cards.empty()) delete cards.front(), cards.pop_front();
	return true;
}

bool CardManager::PostUpdate()
{
	if (to_delete)
	{
		for (std::list<Card*>::iterator card = cards.begin(); card != cards.end(); ++card)
		{
			if ((*card)->to_delete)
			{
				App->tex->UnLoad((*card)->sprite_path);
				delete (*card);
				card = cards.erase(card);
			}				
		}

		to_delete = false;
	}
	return true;
}

bool CardManager::Awake(pugi::xml_node& conf)
{
	pugi::xml_parse_result result = config_file.load_file("xml/cards.xml");

	if (result == NULL)
		LOG("Could not load card xml file. pugi error: %s", result.description());
	else
		card_configs = config_file.child("config");

	return true;
}

bool CardManager::Start()
{
	return true;
}

Card* CardManager::CreateCard(EntityType type)
{
	Card* card = new Card;
	card->type = type;
	card->level = 0;

	pugi::xml_node card_node = card_configs.find_child_by_attribute("type", std::to_string((int)type).c_str());

	card->name = card_node.child("name").child_value();
	card->sprite_path = card_node.child("sprite").child_value();

	LoadCardStats(card, card_node.child("stats"));
	cards.push_back(card);

	return card;
}

Card* CardManager::DeleteCard(Card* card)
{
	card->to_delete = true;
	to_delete = true;

	return nullptr;
}

void CardManager::LoadCardStats(Card* card, pugi::xml_node stats_node)
{
	for (pugi::xml_node iter = stats_node.child("stat"); iter; iter = iter.next_sibling("stat"))
	{
		std::string stat_name = iter.attribute("stat").as_string();

		//Create the stat
		card->info.stats.insert(std::pair<std::string, Stat*>(
			stat_name,
			new Stat(iter.attribute("value").as_int())));
	}

	card->info.attack_type = (AttackType)stats_node.attribute("attack_type").as_uint();
	card->info.armored = stats_node.attribute("armored").as_bool();
}

void Card::Upgrade()
{
	level++;
	info.stats.find("health")->second->IncreaseMaxValue(info.scaling.health_upgrade);
	info.stats.find("damage")->second->IncreaseMaxValue(info.scaling.attack_damage_upgrade);
	info.stats.find("defense")->second->IncreaseMaxValue(info.scaling.defense_upgrade);
	info.stats.find("movement")->second->IncreaseMaxValue(info.scaling.movement_speed_upgrade);
	info.stats.find("attack_speed")->second->IncreaseMaxValue(info.scaling.attack_speed_upgrade);
	info.stats.find("range")->second->IncreaseMaxValue(info.scaling.range_upgrade);
}
