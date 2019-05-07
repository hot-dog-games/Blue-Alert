#include "BuildingBuff.h"


BuildingBuff::BuildingBuff(BUFF_TYPE type, std::string stat, float value, uint source_id, BuildingBuffType b_type) : Buff(type, stat, value, source_id)
{
	building_buff = b_type;
}

BuildingBuff::~BuildingBuff()
{
}

