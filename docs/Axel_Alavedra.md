# Axel Alavedra

## Individual Contribution


### Entity System
* Implemented an entity system that splits entities into Dynamic and Static. Dynamic entities are all the units of the game, Static entities are the buildings that you capture and try to destroy.
* Implemented Entity Factoy which creates units related to a card (the card gives the stats to the entity like movement speed, damage, etc).
* Core entity, which contains a deck of cards, spawns units using this cards and is the objective of the battle.
### Card/Deck System
* Implemented card system, a card contains information about unit type and all it's stats like movement speed, attack damage, life, and many others.
* Card factory that reads card stats from xml and creates card according to type wanted.
* Deck of cards for the player and the enemy. Collection of cards for the player, which contains all the cards that the player obtains during the game.
* Card scales stats depending on level
### Units
* Implemented unit spawn through card usage
* Limited spawn of units inside spawn area
* Unit movement through A* pathfinding
* Animations
	* Adjust animations pivot for units
	* Movement and attack animations frame speeds are related to the movement speed and attack speed of the unit
	* Unit dynamic animations
* Implemented Combat
	* Implemented the 3 different attack types in the game (Basic attack, Piercing attack and AOE attack)
	* Particles for each type of attack(AOE explosion, bullet for basic and piercing)
	* Shot particle rotates and moves to enemy position
	* Player ultimate ability
* Basic AI of units
	* Unit moves to the enemy Core
	* If an Entity enters in attack range, starts attacking that unit
	* After killing enemy, looks for more entities in range, if no entity is in range, continues moving to enemy Core.
### Enemy AI
* Implemented a basic opponent AI that decides what to play depending on situation.
* Select appropriate counter to opponent units
* Prioritizes zone of the map to spawn depending on amount of enemies, damage and proximity to Core.
### Save/Load
* Game automatically saves each time a battle has finished.
* If player loses, the game recovers the state at which the current stage was started and loads it.
* Load save file function
### Other tasks
* Adapted the base code to STL.
* Helped with energy bar
* Fixed all major bugs and memory leaks
* Optimized texture and audio loading.
* Implemented color change on life bars depending on value
* Implemented Video module to the engine from my research (link)
* Helped as much as I could with code issues of other team members
