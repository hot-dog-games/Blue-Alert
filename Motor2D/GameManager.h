#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "p2Defs.h"
#include "Module.h"

struct Card;
class Deck;

class GameManager : public Module
{
public:
	GameManager();
	~GameManager();
private:
	uint gold;
	std::list<Card*> collection;
	Deck* combat_deck;
};

#endif // !_GAME_MANAGER_H_