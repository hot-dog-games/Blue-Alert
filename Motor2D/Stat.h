#ifndef _STAT_H_
#define _STAT_H_

#include <vector>

#include "p2Defs.h"

class Buff;

class Stat {
private:
	float final_value;
	std::vector<Buff*> additive_buffs;
	std::vector<Buff*> multiplicative_buffs;

public:
	float base_value;//INFO: The value of the stat without adding any buff
	Stat(float base);

	void AddBuff(Buff & buff);
	void RemoveBuff(uint source_id);
	void CalculateStat();
	float GetValue();
};

#endif // !_STAT_H_
