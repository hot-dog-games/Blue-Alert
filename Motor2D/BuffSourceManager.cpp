#include "p2Log.h"
#include "BuffSourceManager.h"



BuffSourceManager::BuffSourceManager()
{
}


BuffSourceManager::~BuffSourceManager()
{
}

bool BuffSourceManager::Awake(pugi::xml_node &)
{
	pugi::xml_parse_result result = buff_config.load_file("xml/buffs.xml");

	if (result == NULL)
		LOG("Could not load buff xml. pugi error: %s", result.description());
	else
		buff_node = buff_config.child("config");

	return true;
}

uint BuffSourceManager::GetNewSourceID()
{
	return last_source_id++;
}

BUFF_TYPE BuffSourceManager::GetBuffType(std::string buff_type)
{
	if (std::strcmp(buff_type.c_str(), "additive") == 0) {
		return BUFF_TYPE::ADDITIVE;
	}
	else if (std::strcmp(buff_type.c_str(), "multiplicative") == 0) {
		return BUFF_TYPE::MULTIPLICATIVE;
	}
	else {
		LOG("Buff type not found.");
	}
}

BuffSource* BuffSourceManager::CreateBuffSource(std::string source_name)
{
	pugi::xml_node source_node = buff_node.find_child_by_attribute("name", source_name.c_str());

	if (source_node != NULL)
	{
		BuffSource* buff_source = nullptr;
		std::string type = source_node.attribute("type").as_string();
		if (type == "leveled_upgrade")
		{
			buff_source = new LeveledUpgrade(source_node);
		}
		return buff_source;
	}
	return nullptr;
}
