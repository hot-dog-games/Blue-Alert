#ifndef _STATIC_ENTITY_H_
#define _STATIC_ENTITY_H_

#include "Entity.h"

enum Static_State {
	STATIC_IDLE,
	STATIC_DIE,
	STATIC_DESTROYED
};


class StaticEntity : public Entity
{
public:
	StaticEntity();
	StaticEntity(pugi::xml_node entity_node, fPoint position);
	~StaticEntity();

	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool CleanUp() { return true; };

	virtual void Die();

private:
	Static_State state;
};

#endif // _STATIC_ENTITY_H_



