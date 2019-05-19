#include "j1App.h"
#include "BuffSourceManager.h"
#include "Stat.h"
#include "Buff.h"



Buff::Buff(BUFF_TYPE type, std::string stat, float value, uint source_id) :
	type(type),
	stat(stat),
	value(value),
	source_id(source_id)
{
}

BUFF_TYPE Buff::GetType()
{
	return type;
}

std::string Buff::GetStat() {
	return stat;
}

float Buff::GetValue()
{
	return value;
}

uint Buff::GetSource()
{
	return source_id;
}

bool Buff::IsCausedBySource(uint source_id) {
	return this->source_id == source_id;
}

BuffSource::BuffSource(pugi::xml_node source_node)
{
	source_id = App->buff->GetNewSourceID();
	for (pugi::xml_node iter = source_node.child("buff"); iter; iter = iter.next_sibling("buff"))
	{
		buffs.push_back(new Buff(
			App->buff->GetBuffType(iter.attribute("type").as_string()),
			iter.attribute("stat").as_string(),
			iter.attribute("value").as_float(),
			source_id));
	}
}

void BuffSource::CleanUp()
{
	for (int i = 0; i < buffs.size(); i++)
	{
		delete buffs[i];
	}
	buffs.clear();
}

void BuffSource::RemoveBuffs(std::map<std::string, Stat*> stats)
{
	std::map<std::string, Stat*>::iterator item;
	for (item = stats.begin(); item != stats.end(); ++item)
	{
		item->second->RemoveBuff(source_id);
	}
}

void LeveledUpgrade::GetBuffs(std::map<std::string, Stat*> stats)
{
	for (uint i = 0; i < buff_amount; ++i)
	{
		uint position = i + (level * buff_amount);
		std::string stat_name = buffs[position]->GetStat();
		stats[stat_name]->AddBuff(*buffs[position]);
		stats[stat_name]->CalculateStat();
	}
}

uint LeveledUpgrade::GetCost()
{
	if (level < max_level)
		return base_cost * (level + 1);
	else 
		return -1;
}

LeveledUpgrade::LeveledUpgrade(pugi::xml_node source_node) :BuffSource(source_node)
{
	base_cost = source_node.child("info").attribute("base_cost").as_uint();
	max_level = source_node.child("info").attribute("max_level").as_uint();
	buff_amount = source_node.child("info").attribute("buff_amount").as_uint();
}

bool LeveledUpgrade::LevelUp()
{
	if (level < max_level)
	{
		level++;
		return true;
	}
	else
		return false;
}