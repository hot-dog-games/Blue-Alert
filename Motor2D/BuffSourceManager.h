#ifndef _BUFF_SOURCE_MANAGER_H_
#define _BUFF_SOURCE_MANAGER_H_

#include "Buff.h"
#include "p2Defs.h"
#include "Module.h"

class BuffSourceManager : public Module
{
public:
	BuffSourceManager();
	~BuffSourceManager();

	bool Awake(pugi::xml_node &) override;
	bool CleanUp();

	uint GetNewSourceID();
	BUFF_TYPE GetBuffType(std::string);
	BuffSource* CreateBuffSource(std::string);

private:
	uint last_source_id = 0u;

	std::vector<BuffSource*> buff_sources;
	pugi::xml_node buff_node;
	pugi::xml_document buff_config;
};


#endif // !_BUFF_SOURCE_MANAGER_H_
