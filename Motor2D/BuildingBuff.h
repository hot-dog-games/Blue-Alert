#ifndef _BUILDINGBUFF_H_
#define _BUILDINGBUFF_H_

#include "Buff.h"

enum class BuildingBuffType{
	NONE,
	INFANTRY_BUFF,
	AERIAL_BUFF,
	LAND_BUFF
};

class BuildingBuff :
	public Buff
{
private:
	BuildingBuffType building_buff;
public:
	BuildingBuff(BUFF_TYPE type, std::string stat, float value, uint source_id, BuildingBuffType b_type);
	~BuildingBuff();
};

#endif

