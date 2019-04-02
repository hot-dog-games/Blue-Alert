#include "j1App.h"
#include "Render.h"
#include "CardManager.h"
#include "Pathfinding.h"
#include "Stat.h"
#include "Map.h"
#include "DynamicEntity.h"



DynamicEntity::DynamicEntity()
{
}


DynamicEntity::~DynamicEntity()
{
}

DynamicEntity::DynamicEntity(pugi::xml_node config, fPoint position, Card* card, Faction faction): Entity(config, position, faction)
{
	entity_card = card;
}

bool DynamicEntity::PostUpdate()
{
	App->render->Blit(sprite, position.x, position.y, &current_frame);

	return true;
}

bool DynamicEntity::PreUpdate()
{

	switch (state)
	{
	case DYNAMIC_IDLE:
		break;
	case DYNAMIC_MOVING:
		CheckDestination();
		CalcDirection();

		break;
	case DYNAMIC_ATTACKING:
		break;
	case DYNAMIC_DYING:
		break;
	}

	return true;
}

bool DynamicEntity::Update(float dt)
{
	switch (state)
	{
		case DYNAMIC_IDLE:
		{
			fPoint core_position = App->entity_manager->GetCorePosition(faction);
			iPoint map_position = App->map->WorldToMap(position.x, position.y);
			App->pathfinding->CreatePath({ (int)map_position.x,(int)map_position.y }, { (int)core_position.x ,(int)core_position.y });
			path = App->pathfinding->GetLastPath();
			state = DYNAMIC_MOVING;
		}
		break;
		case DYNAMIC_MOVING:
		{
			Move(dt);
		}
		break;
		case DYNAMIC_ATTACKING:
		{

		}
		break;
		case DYNAMIC_DYING:
		{

		}
		break;
	}

	current_frame = animations[state].GetCurrentFrame(dt);

	return true;
}

void DynamicEntity::CalcDirection()
{
	map_position = App->map->WorldToMap(position.x, position.y);
	if (map_position.x < path[current_point].x)
	{
		direction = RIGHT;
		if (map_position.y < path[current_point].y)
		{
			direction = DOWN_RIGHT;
		}
		else if (map_position.y > path[current_point].y)
		{
			direction = UP_RIGHT;
		}
	}
	else if (map_position.x > path[current_point].x)
	{
		direction = LEFT;
		if (map_position.y < path[current_point].y)
		{
			direction = DOWN_LEFT;
		}
		else if (map_position.y > path[current_point].y)
		{
			direction = UP_LEFT;
		}
	}
	else if (map_position.y < path[current_point].y && direction != RIGHT && direction != LEFT && direction != UP_RIGHT 
		&& direction != UP_LEFT && direction != DOWN_RIGHT && direction != DOWN_LEFT)
	{
		direction =	DOWN;
	}
	else if (map_position.y > path[current_point].y && direction != RIGHT && direction != LEFT && direction != UP_RIGHT
		&& direction != UP_LEFT && direction != DOWN_RIGHT && direction != DOWN_LEFT)
	{
		direction = UP;
	}
}

void DynamicEntity::CheckDestination()
{
	map_position = App->map->WorldToMap(position.x, position.y);
	bool reached_x = (path[previous_point].x <= path[current_point].x && path[current_point].x <= map_position.x)
		|| (path[previous_point].x >= path[current_point].x && path[current_point].x >= map_position.x);

	bool reached_y = (path[previous_point].y <= path[current_point].y && path[current_point].y <= map_position.y)
		|| (path[previous_point].y >= path[current_point].y && path[current_point].y >= map_position.y);

	if (reached_x && reached_y)
	{
		previous_point = current_point;
		current_point++;
		if (current_point > path.size())
			state = DYNAMIC_IDLE;
	}
}

void DynamicEntity::Move(float dt)
{
	float speed = entity_card->info.stats.find("movement")->second->GetValue()*10;
	
	switch (direction)
	{
	case UP:
		position.y -= speed*dt;
		break;
	case DOWN:
		position.y += speed * dt;
		break;
	case LEFT:
		position.x -= speed * dt;
		break;
	case RIGHT:
		position.x += speed * dt;
		break;
	case UP_RIGHT:
		position.y -= speed * dt;
		position.x += speed * dt;
		break;
	case UP_LEFT:
		position.y -= speed * dt;
		position.x -= speed * dt;
		break;
	case DOWN_RIGHT:
		position.y += speed * dt;
		position.x += speed * dt;
		break;
	case DOWN_LEFT:
		position.y += speed * dt;
		position.x -= speed * dt;
		break;
	}
	
}