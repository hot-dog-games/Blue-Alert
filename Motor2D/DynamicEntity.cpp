#include "p2Log.h"
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
	std::string stat_name = "health";
	stats.insert({ "health", new Stat(card->info.stats.find("health")->second->GetMaxValue())});
}

bool DynamicEntity::Start()
{
	fPoint core_position = App->entity_manager->GetCorePosition(faction);
	iPoint core_map_position = App->map->WorldToMap((int)core_position.x, (int)core_position.y);
	iPoint map_position = App->map->WorldToMap(position.x, position.y);

	App->pathfinding->CreatePath({ map_position.x, map_position.y }, { core_map_position.x , core_map_position.y });
	path = App->pathfinding->GetLastPath();
	state = DYNAMIC_MOVING;

	for (std::vector<iPoint>::iterator point = path.begin(); point != path.end(); ++point)
	{
		*point = App->map->MapToWorld((*point).x, (*point).y);
		*point = { (*point).x + (int)(App->map->data.tile_width * 0.5), (*point).y + (int)(App->map->data.tile_height * 0.5) };
	}

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
		CheckEnemies();
		break;
	case DYNAMIC_ATTACKING:
		if (!objective->IsAlive())
		{
			state = DYNAMIC_MOVING;
			objective = nullptr;
			CheckEnemies();
		}
		break;
	case DYNAMIC_DYING:
		if (animations[state].isDone())
		{
			state = DYNAMIC_DEAD;
		}
		break;
	}

	return true;
}

bool DynamicEntity::PostUpdate()
{
	fPoint render_position = { position.x - (current_frame.w * 0.5f), position.y - current_frame.h };
	App->render->Blit(sprite, render_position.x, render_position.y, &current_frame);


	//Range debug 
	App->render->DrawCircle(position.x, position.y, entity_card->info.stats.find("range")->second->GetValue()*App->map->data.tile_height, 255, 0, 0);

	return true;
}

bool DynamicEntity::Update(float dt)
{
	switch (state)
	{
		case DYNAMIC_IDLE:
		{

		}
		break;
		case DYNAMIC_MOVING:
		{
			Move(dt);
		}
		break;
		case DYNAMIC_ATTACKING:
		{
			Attack();
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
	if (position.x < path[current_point].x)
	{
		direction = RIGHT;
		if (position.y < path[current_point].y)
		{
			direction = DOWN_RIGHT;
		}
		else if (position.y > path[current_point].y)
		{
			direction = UP_RIGHT;
		}
	}
	else if (position.x > path[current_point].x)
	{
		direction = LEFT;
		if (position.y < path[current_point].y)
		{
			direction = DOWN_LEFT;
		}
		else if (position.y > path[current_point].y)
		{
			direction = UP_LEFT;
		}
	}
	else if (position.y < path[current_point].y)
	{
		direction =	DOWN;
	}
	else if (position.y > path[current_point].y)
	{
		direction = UP;
	}
}

void DynamicEntity::CheckDestination()
{
	bool reached_x = (path[previous_point].x <= path[current_point].x && path[current_point].x <= position.x)
		|| (path[previous_point].x >= path[current_point].x && path[current_point].x >= position.x);

	bool reached_y = (path[previous_point].y <= path[current_point].y && path[current_point].y <= position.y)
		|| (path[previous_point].y >= path[current_point].y && path[current_point].y >= position.y);

	if (reached_x && reached_y)
	{
		previous_point = current_point;
		current_point++;
		if (current_point >= path.size())
			state = DYNAMIC_IDLE;
	}
}

void DynamicEntity::Move(float dt)
{
	fPoint move_pos = { path[current_point].x - position.x, path[current_point].y - position.y };

	float m = sqrtf(pow(move_pos.x, 2.0f) + pow(move_pos.y, 2.0f));

	if (m > 0.0f) {
		move_pos.x /= m;
		move_pos.y /= m;
	}

	float speed = entity_card->info.stats.find("movement")->second->GetValue() * 10;
	move_pos.x *= speed * dt;
	move_pos.y *= speed * dt;

	position.x += move_pos.x;
	position.y += move_pos.y;
	
}

void DynamicEntity::CheckEnemies()
{
	Entity* closest_entity = nullptr;
	float distance = 10000.0f;
	App->entity_manager->FindClosestEnemy(position, faction, closest_entity, distance);
	LOG("distance to closest is %f radius is %f", distance, entity_card->info.stats.find("range")->second->GetValue()*App->map->data.tile_height);

	if (distance <= entity_card->info.stats.find("range")->second->GetValue()* App->map->data.tile_height && closest_entity->IsAlive())
	{
		objective = closest_entity;
		state = DYNAMIC_ATTACKING;
	}
}


void DynamicEntity::Attack()
{
	if (attack_timer.ReadMs() >= SECOND_MS / entity_card->info.stats.find("attack_speed")->second->GetValue() )
	{
		objective->DecreaseLife(entity_card->info.stats.find("damage")->second->GetValue());
		attack_timer.Start();
	}
}

void DynamicEntity::Die()
{
	state = DYNAMIC_DYING;
	objective = nullptr;
	path.clear();
}