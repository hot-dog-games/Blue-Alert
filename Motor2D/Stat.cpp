#include <algorithm>

#include "p2Log.h"
#include "j1App.h"
#include "BuffSourceManager.h"
#include "Buff.h"
#include "Stat.h"


Stat::Stat(float base) :
	base_value(base),
	final_value(base),
	max_value(base)
{
}

void Stat::AddBuff(Buff & buff)
{
	switch (buff.GetType())
	{
	case BUFF_TYPE::ADDITIVE:
		additive_buffs.push_back(&buff);
		break;
	case BUFF_TYPE::MULTIPLICATIVE:
		multiplicative_buffs.push_back(&buff);
		break;
	default:
		LOG("Buff type not detected.");
		break;
	}
}

//Searches through all the buffs and removes the ones caused by the source
void Stat::RemoveBuff(uint source_id)
{
	additive_buffs.erase(std::remove_if(
		additive_buffs.begin(),
		additive_buffs.end(),
		[source_id](Buff * buff) { return buff->IsCausedBySource(source_id); }),
		additive_buffs.end());

	multiplicative_buffs.erase(std::remove_if(
		multiplicative_buffs.begin(),
		multiplicative_buffs.end(),
		[source_id](Buff * buff) { return buff->IsCausedBySource(source_id); }),
		multiplicative_buffs.end());
}

void Stat::CalculateStat()
{
	final_value = base_value;

	//1. Apply addtive buffs
	for (std::vector<Buff*>::iterator iter = additive_buffs.begin(); iter != additive_buffs.end(); ++iter)
	{
		final_value += (*iter)->GetValue();
	}

	//2. Add multiplicative buffs and calculate the percentage
	float totalMult = 0.f;
	for (std::vector<Buff*>::iterator iter = multiplicative_buffs.begin(); iter != multiplicative_buffs.end(); ++iter)
	{
		totalMult += (*iter)->GetValue();
	}
	final_value += totalMult * final_value;
}

float Stat::GetValue()
{
	return final_value;
}

float Stat::GetMaxValue()
{
	return max_value;
}

void Stat::DecreaseStat(float value)
{
	base_value -= value;
	if (base_value < 0)
		base_value = 0;
	CalculateStat();
}

void Stat::IncreaseStat(float value)
{
	base_value += value;
	if (base_value > max_value)
		base_value = max_value;
	CalculateStat();
}
