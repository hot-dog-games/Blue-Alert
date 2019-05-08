#ifndef _BuildingBuffSource_H_
#define _BuildingBuffSource_H_

#include "Buff.h"


class BuildingBuffSource :
	public BuffSource
{
private:
	bool is_active = false;

private:
	void(*function_ptr)(BuildingBuffSource *) = nullptr;
public:
	BuildingBuffSource(pugi::xml_node buff_source_node);
	~BuildingBuffSource();
};

#endif

