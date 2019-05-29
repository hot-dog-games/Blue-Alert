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
				cards.erase(card);
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
	card->buff_type = (EntityType)card_node.child("buff_type").attribute("value").as_int();

	LoadCardStats(card, card_node.child("stats"));
	LoadCardUpgrades(card, card_node.child("upgrades"));
	LoadCardCombat(card, card_node.child("combat"));
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
}

void CardManager::LoadCardCombat(Card* card, pugi::xml_node combat_node)
{
	card->info.attack_type = (AttackType)combat_node.attribute("attack_type").as_uint();
	card->info.armored = combat_node.attribute("armored").as_bool();
}

void CardManager::LoadCardUpgrades(Card * card, pugi::xml_node upgrades_node)
{
	card->info.scaling.health_upgrade = upgrades_node.find_child_by_attribute("stat", "health").attribute("value").as_uint();
	card->info.scaling.attack_damage_upgrade = upgrades_node.find_child_by_attribute("stat", "damage").attribute("value").as_uint();
	card->info.scaling.defense_upgrade = upgrades_node.find_child_by_attribute("stat", "defense").attribute("value").as_uint();
	card->info.scaling.movement_speed_upgrade = upgrades_node.find_child_by_attribute("stat", "movement").attribute("value").as_uint();
	card->info.scaling.attack_speed_upgrade = upgrades_node.find_child_by_attribute("stat", "attack_speed").attribute("value").as_uint();
	card->info.scaling.range_upgrade = upgrades_node.find_child_by_attribute("stat", "range").attribute("value").as_uint();
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

void Card::LoadSprite()
{
	if (sprite_path != " ")
	{
		texture = App->tex->Load(sprite_path.c_str());
	}
}