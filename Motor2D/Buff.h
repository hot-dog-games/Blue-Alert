#ifndef _BUFF_H_
#define _BUFF_H_

#include <string>
#include <list>

#include "PugiXml/src/pugixml.hpp"
#include "p2Defs.h"


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

public:
	uint source_id;
	std::list<Buff*> buffs;
};

#endif //_BUFF_H_