#ifndef _BUFF_H_
#define _BUFF_H_

#include <string>
#include <vector>
#include <map>

#include "PugiXml/src/pugixml.hpp"
#include "p2Defs.h"

class Stat;

enum class BUFF_TYPE {
	ADDITIVE, //AKA: Flat, raw
	MULTIPLICATIVE, // AKA: Percent
	MAX
};

class Buff {
public:
	Buff(BUFF_TYPE type, std::string stat, float value, uint source_id);
	BUFF_TYPE GetType();
	std::string GetStat();
	float GetValue();
	uint GetSource();
	bool IsCausedBySource(uint source_id);

private:
	BUFF_TYPE type = BUFF_TYPE::MAX;
	std::string stat = "\0";
	float value = 0.f;
	uint source_id = 0u; //ID from which modifier (object, spell, etc) the buff came from
};

class BuffSource 
{
public:
	BuffSource(pugi::xml_node buff_source_node);
	virtual void GetBuffs(std::map<std::string, Stat*> stats) {};
	void CleanUp();
	void RemoveBuffs(std::map<std::string, Stat*> stats);
public:
	uint source_id;
	std::vector<Buff*> buffs;
};

class LeveledUpgrade : public BuffSource
{
public:
	LeveledUpgrade(pugi::xml_node buff_source_node);
	void GetBuffs(std::map<std::string, Stat*> stats);
	uint GetCost();
	bool LevelUp();
	void SetLevel(int lvl);
	void Reset();
	uint GetLevel() {
		return level;
	}
	uint GetBuffValue(std::string name);
private:
	uint max_level = 0;
	uint level = 0;
	uint base_cost = 0;
	uint buff_amount = 0;
};

#endif //_BUFF_H_