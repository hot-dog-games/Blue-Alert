#include "j1App.h"
#include "BuffSourceManager.h"
#include "Buff.h"



Buff::Buff(BUFF_TYPE type, std::string stat, float value, uint source_id) :
	type(type),
	stat(stat),
	value(value),
	source_id(source_id) {}

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
	for (pugi::xml_node iter = source_node.child("buff"); iter; iter = iter.next_sibling("buff"))
	{
		buffs.push_back(new Buff(
			App->buff->GetBuffType(iter.attribute("type").as_string()),
			iter.attribute("stat").as_string(),
			iter.attribute("value").as_float(),
			source_id));
	}
}