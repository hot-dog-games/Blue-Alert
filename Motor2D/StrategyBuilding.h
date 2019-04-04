#pragma once
#include "StaticEntity.h"

class StrategyBuilding :
	public StaticEntity
{
public:
	enum class State {
		INRANGE,
		HIGHLIGHTED,
		OFFRANGE
	};

	StrategyBuilding();
	~StrategyBuilding();

};

