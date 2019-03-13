#ifndef _DYNAMIC_ENTITY_
#define _DYNAMIC_ENTITY_

#include "Entity.h"

struct Card;

enum DynamicState {
	DYNAMIC_IDLE,
	DYNAMIC_MOVING,
	DYNAMIC_ATTACKING,
	DYNAMIC_DYING
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

	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };

	virtual void Die() {};
	virtual void Move() {};
	virtual void Attack() {};

	void SetCard(Card* card);

protected:
	DynamicState state;
	EntiyDirection direction;
	Card* entity_card;
};

#endif // !_DYNAMIC_ENTITY_



