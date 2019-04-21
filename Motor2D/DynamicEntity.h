#ifndef _DYNAMIC_ENTITY_
#define _DYNAMIC_ENTITY_

#include "Entity.h"

struct Card;
struct SingleUnit;

enum DynamicState {
	DYNAMIC_IDLE,
	DYNAMIC_MOVING,
	DYNAMIC_ATTACKING,
	DYNAMIC_DYING,
	DYNAMIC_DEAD
};

enum EntiyDirection {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_RIGHT,
	UP_LEFT,
	DOWN_RIGHT,
	DOWN_LEFT
};

class DynamicEntity : public Entity
{
public:
	DynamicEntity();
	~DynamicEntity();
	DynamicEntity(pugi::xml_node config, fPoint position, Card* card, Faction faction);

	virtual bool PreUpdate();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp();
	virtual bool Start();
	SingleUnit* GetSingleUnit();
	void SetUnitDirection(EntiyDirection unitDirection);
	EntiyDirection GetUnitDirection();
	void SetUnitDirectionByValue(fPoint unitDirection);
	fPoint GetUnitDirectionByValue() const;

protected:
	void CalcDirection();
	void CheckDestination();
	virtual void AnimationCheck();
	virtual void MovementAnimationCheck();
	virtual void AttackingAnimationCheck();
	virtual void Die();
	virtual void Move(float dt);
	virtual void Attack();
	bool CheckEnemies();
	bool CheckAllies();

public:
	Card * entity_card = nullptr;

protected:
	DynamicState state = DYNAMIC_IDLE;
	EntiyDirection direction = UP;
	fPoint direction_vector = { 1.0f, 1.0f };
	SingleUnit * singleUnit = nullptr;

	const float SNAP_RANGE = 5.0f;

	Entity* objective = nullptr;
	PerfTimer attack_timer;

	//Pathfinding
	std::vector<iPoint> path;
	SDL_Rect pivot;
	int current_point = 0;
	int previous_point = 0;
};



#endif // !_DYNAMIC_ENTITY_



