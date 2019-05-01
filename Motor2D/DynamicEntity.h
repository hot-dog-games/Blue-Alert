#ifndef _DYNAMIC_ENTITY_
#define _DYNAMIC_ENTITY_

#include "Entity.h"

struct Card;
struct SingleUnit;
class UIBar;

enum DynamicState {
	DYNAMIC_IDLE,
	DYNAMIC_MOVING,
	DYNAMIC_ATTACKING,
	DYNAMIC_DYING,
	DYNAMIC_DEAD,
	DYNAMIC_GETPATH,
	DYNAMIC_INCREMENTWAYPOINT

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
	virtual void DecreaseLife(float damage, bool piercing = false);
	virtual float GetDamage();
	virtual bool IsArmored();
	virtual int GetAttackType();
	virtual bool MoveOfTheWayOf(DynamicEntity* u);

public:
	Card* entity_card = nullptr;
	iPoint goal;
	iPoint next_Goal;
	DynamicEntity* waitingUnit = nullptr;

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

protected:

	DynamicState state = DYNAMIC_IDLE;
	EntiyDirection direction = UP;
	fPoint direction_vector = { 1.0f, 1.0f };
	SingleUnit* singleUnit = nullptr;

	const float SNAP_RANGE = 3.0f;

	Entity* objective = nullptr;
	PerfTimer attack_timer;

	//Pathfinding
	std::vector<iPoint> path;
	SDL_Rect pivot;
	int current_point = 0;
	int previous_point = 0;
	float dead_timer = 0.0f;

private:
	std::string attack_fx;
	std::string explosion_fx;
	
	UIBar*	health_bar = nullptr;
};



#endif // !_DYNAMIC_ENTITY_



