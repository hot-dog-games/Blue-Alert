#include "p2Log.h"
#include "j1App.h"
#include "Render.h"
#include "Audio.h"
#include "Movement.h"
#include "GUI.h"
#include "UIBar.h"
#include "CardManager.h"
#include "Pathfinding.h"
#include "Stat.h"
#include "Map.h"
#include "Particles.h"
#include "DynamicEntity.h"

const float DELETE_TIME = 5.0f;
const int EXPLOSION_RANGE_TILES = 2;

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
	sprite = entity_card->texture;

	singleUnit = new SingleUnit(this, nullptr);
	App->movement->CreateGroupFromUnit(this);

	stats.insert({ "health", new Stat(card->info.stats.find("health")->second->GetMaxValue())});
}

bool DynamicEntity::Start()
{
	fPoint core_position = App->entity_manager->GetCorePosition(faction);
	iPoint core_map_position = App->map->WorldToMap((int)core_position.x, (int)core_position.y);
	iPoint map_position = App->map->WorldToMap(position.x, position.y);
	iPoint bar_position = App->render->WorldToScreen(position.x, position.y);

	App->pathfinding->CreatePath({ map_position.x, map_position.y }, { core_map_position.x , core_map_position.y });
	path = App->pathfinding->GetLastPath();
	App->movement->GetGroupByUnit(this)->SetGoal(path[path.size()-1]);

	state = DYNAMIC_MOVING;
	current_animation = &animations.find("moving_up")->second;
	current_frame = current_animation->GetCurrentFrame(0.1);

	for (std::vector<iPoint>::iterator point = path.begin(); point != path.end(); ++point)
	{
		*point = App->map->MapToWorld((*point).x, (*point).y);
		*point = { (*point).x + (int)(App->map->data.tile_width * 0.5), (*point).y + (int)(App->map->data.tile_height * 0.5) };
	}
	explosion_fx = App->audio->LoadFx("audio/fx/Ambient_Sounds/Explosions/Explosion2.wav");
	attack_fx = App->audio->LoadFx("audio/fx/Ambient_Sounds/Shots/One_shoot2.wav");

	App->audio->SetFXVolume(attack_fx.c_str(), 30);
	App->audio->SetFXVolume(explosion_fx.c_str(), 30);

	health_bar = App->gui->CreateBar(bar_position, { 25,1503,current_frame.w, 7 }, stats.find("health")->second, BarType::BAR_HORITZONTAL, this);
	return true;
}

bool DynamicEntity::PreUpdate()
{
	switch (state)
	{
	case DYNAMIC_IDLE:
		break;
	case DYNAMIC_MOVING:
		if (CheckEnemies())
			state = DYNAMIC_ATTACKING;
		else 
			CheckDestination();
		break;
	case DYNAMIC_ATTACKING:
		if (!objective->IsAlive() && current_animation->isDone())
		{
			objective = nullptr;
			if (!CheckEnemies())
			{
				state = DYNAMIC_MOVING;
				CheckDestination();
			}			
		}
		else
		{
			fPoint objective_direction = { objective->position.x - position.x, objective->position.y - position.y };
			float m = sqrtf(pow(objective_direction.x, 2.0f) + pow(objective_direction.y, 2.0f));
			if (m > 0.0f) {
				objective_direction.x /= m;
				objective_direction.y /= m;
			}

			direction_vector = { objective_direction.x, objective_direction.y };
		}
		break;
	case DYNAMIC_DYING:
		if (current_animation->isDone())
			state = DYNAMIC_DEAD;
		break;
	}

	CalcDirection();
	AnimationCheck();
	pivot.x = position.x - current_frame.w / 2; pivot.y = position.y; pivot.w = current_frame.w; pivot.h = -current_frame.h / 2;
	return true;
}

bool DynamicEntity::PostUpdate()
{
	Draw();

	if (debug)
	{
		//Range debug 
		App->render->DrawCircle(position.x, position.y, entity_card->info.stats.find("range")->second->GetValue()*App->map->data.tile_height, 255, 0, 0);
		App->render->DrawQuad(pivot, 255, 0, 0);
	}

	return true;
}

bool DynamicEntity::Update(float dt)
{
	if (current_animation)
		current_frame = current_animation->GetCurrentFrame(dt);

	switch (state)
	{
		case DYNAMIC_IDLE:
		{
			if (next_Goal != waitingUnit->next_Goal && next_Goal != App->map->WorldToMap(waitingUnit->position.x, waitingUnit->position.y))
			{
				if (path.size() > 0)
					state = DYNAMIC_MOVING;
				else
					state = DYNAMIC_GETPATH;
			}
		}
		break;
		case DYNAMIC_MOVING:
		{
			Move(dt);
		}
		break;
		case DYNAMIC_ATTACKING:
		{
			if(objective->IsAlive())
				Attack();
		}
		break;
		case DYNAMIC_DYING:
		{

		}
		break;
		case DYNAMIC_DEAD:
			dead_timer += dt;
			if (dead_timer >= DELETE_TIME)
			{
				App->entity_manager->DeleteEntity(this);
			}
			break;
		case DYNAMIC_GETPATH:
		{
			int result = App->pathfinding->CreatePath(App->map->WorldToMap((int)position.x, (int)position.y), goal);
			if (result != -1)
			{
				path.clear();
				path = App->pathfinding->GetLastPath();
				state = DYNAMIC_INCREMENTWAYPOINT;
				path.erase(path.begin());

			}
			else
			{
				state = DYNAMIC_IDLE;
			}
		}
		break;
		case DYNAMIC_INCREMENTWAYPOINT:
		{

			next_Goal = *path.begin();

			iPoint aux = next_Goal - App->map->WorldToMap((int)position.x, (int)position.y);
			SetUnitDirectionByValue({ (float)aux.x,(float)aux.y });
			state = DYNAMIC_MOVING;

			if (path.size() == 0)
				state = DYNAMIC_IDLE;

		}
		break;
	}

	return true;
}

bool DynamicEntity::MoveOfTheWayOf(DynamicEntity* u)
{
	iPoint tile_pos = App->map->WorldToMap((int)position.x, (int)position.y);
	iPoint neightBoar[4];
	neightBoar[UP] = {tile_pos.x, tile_pos.y - 1}; //up
	neightBoar[DOWN] = { tile_pos.x,tile_pos.y + 1 }; //down
	neightBoar[LEFT] = { tile_pos.x - 1,tile_pos.y }; //left
	neightBoar[RIGHT] = { tile_pos.x + 1,tile_pos.y }; //right

	for (uint i = 0; i < 4; ++i)
	{
		if (App->pathfinding->IsWalkable(neightBoar[i]) != true)
		{
			neightBoar[i].SetToZero();
		}
	}
	//x axis --------------------------------------
	if (u->direction == LEFT || u->direction == RIGHT)
	{
		if (neightBoar[UP].IsZero() != true
			&& App->entity_manager->InThisTile_IsUnits(neightBoar[UP]) == nullptr)
		{
			state = DYNAMIC_GETPATH;
			goal = neightBoar[UP];
			return true;
		}


		else if (neightBoar[DOWN].IsZero() != true
			&& App->entity_manager->InThisTile_IsUnits(neightBoar[DOWN]) == nullptr)
		{
			state = DYNAMIC_GETPATH;
			goal = neightBoar[DOWN];
			return true;
		}

		else
		{
			return false;
		}
	}

	else
	{
		if (neightBoar[LEFT].IsZero() != true
			&& App->entity_manager->InThisTile_IsUnits(neightBoar[LEFT]) == nullptr)
		{
			state = DYNAMIC_GETPATH;
			goal = neightBoar[LEFT];
			return true;
		}

		else if (neightBoar[RIGHT].IsZero() != true
			&& App->entity_manager->InThisTile_IsUnits(neightBoar[LEFT]) == nullptr)
		{
			state = DYNAMIC_GETPATH;
			goal = neightBoar[RIGHT];
			return true;
		}

		else
		{
			return false;
		}
	}


}

void DynamicEntity::CalcDirection()
{
	if (direction_vector.x > 0.3f)
	{
		direction = RIGHT;
		if (direction_vector.y > 0)
		{
			direction = DOWN_RIGHT;
		}
		else if (direction_vector.y < 0)
		{
			direction = UP_RIGHT;
		}
	}
	else if (direction_vector.x < -0.3f)
	{
		direction = LEFT;
		if (direction_vector.y > 0)
		{
			direction = DOWN_LEFT;
		}
		else if (direction_vector.y < 0)
		{
			direction = UP_LEFT;
		}
	}
	else if (direction_vector.y > 0)
	{
		direction =	DOWN;
	}
	else if (direction_vector.y < 0)
	{
		direction = UP;
	}
}

void DynamicEntity::CheckDestination()
{
	//bool reached_x = (path[previous_point].x <= path[current_point].x && path[current_point].x <= position.x)
	//	|| (path[previous_point].x >= path[current_point].x && path[current_point].x >= position.x);

	//bool reached_y = (path[previous_point].y <= path[current_point].y && path[current_point].y <= position.y)
	//	|| (path[previous_point].y >= path[current_point].y && path[current_point].y >= position.y);

	//if (reached_x && reached_y)
	//{
	//	previous_point = current_point;
	//	current_point++;
	//	if (current_point >= path.size())
	//		state = DYNAMIC_IDLE;
	//}
	//fPoint move_pos;
	///*if (CheckAllies())
	//{
	//	move_pos = { path[current_point].x - position.x - 20, path[current_point].y - position.y };
	//}else*/
	//move_pos = { path[current_point].x - position.x, path[current_point].y - position.y };
	//
	//float m = sqrtf(pow(move_pos.x, 2.0f) + pow(move_pos.y, 2.0f));
	//if (m > 0.0f) {
	//	move_pos.x /= m;
	//	move_pos.y /= m;
	//}
	//direction_vector = { move_pos.x, move_pos.y };
}

void DynamicEntity::Move(float dt)
{
	App->movement->MoveUnit(this, dt);
	fPoint speed = {1,1};
	if (path.size() > 0)
	{

		iPoint world_next_pos = App->map->MapToWorld(next_Goal.x, next_Goal.y);
		fPoint aux = { (float)world_next_pos.x + App->map->data.tile_width*0.5F,(float)world_next_pos.y + App->map->data.tile_height*0.5F };


		speed = aux - position;
		iPoint int_pos = { (int)position.x,(int)position.y };
		//Check if we are on the center of the tile
		if (App->map->WorldToMap((int)position.x, (int)position.y) == goal)
		{
			path.erase(path.begin());
			App->map->WorldToMap((int)position.x, (int)position.y) = next_Goal;
			state = DYNAMIC_INCREMENTWAYPOINT;
		}

	}
	else
	{
		state = DYNAMIC_IDLE;
	}
	iPoint aux = { (int)(speed.x*dt * 20),(int)(speed.y*dt * 20) };
	position.x += (float)aux.x;
	position.y += (float)aux.y;
	//posCollider.center = position;

	/*fPoint movement_vector = direction_vector;
	float speed = entity_card->info.stats.find("movement")->second->GetValue();
	movement_vector.x *= speed * dt;
	movement_vector.y *= speed * dt;

	fPoint end_position = { position.x + movement_vector.x, position.y + movement_vector.y };

	if (end_position.DistanceNoSqrt({ (float)path[current_point].x, (float)path[current_point].y }) < SNAP_RANGE)
	{
		position = { (float)path[current_point].x, (float)path[current_point].y };
	}
	else
	{
			position.x += movement_vector.x;
			position.y += movement_vector.y;
	}*/
}

bool DynamicEntity::CheckEnemies()
{
	Entity* closest_entity = nullptr;
	float distance = 10000.0f;
	App->entity_manager->FindClosestEnemy(position, faction, closest_entity, distance);

	if (closest_entity && distance <= entity_card->info.stats.find("range")->second->GetValue()* App->map->data.tile_height)
	{
		objective = closest_entity;
		fPoint objective_direction = { objective->position.x - position.x, objective->position.y - position.y };
		float m = sqrtf(pow(objective_direction.x, 2.0f) + pow(objective_direction.y, 2.0f));
		if (m > 0.0f) {
			objective_direction.x /= m;
			objective_direction.y /= m;
		}

		direction_vector = { objective_direction.x, objective_direction.y };
		return true;
	}

	return false;
}

bool DynamicEntity::CheckAllies()
{
	Entity* closest_entity = nullptr;
	float distance = 10000.0f;
	App->entity_manager->FindClosestAllie(position, faction, closest_entity, distance);
	

	if (distance <= current_frame.h/2)
	{
		return true;
	}

	return false;
}


void DynamicEntity::Attack()
{
	if (attack_timer.ReadMs() >= SECOND_MS / entity_card->info.stats.find("attack_speed")->second->GetValue() )
	{
		float attack = entity_card->info.stats.find("damage")->second->GetValue();
		switch (entity_card->info.attack_type)
		{
		case AttackType::AT_BASIC:
			objective->DecreaseLife(attack);
			App->particles->CreateParticle(ParticleType::ATTACK_BASIC_SHOT, { position.x, position.y - current_frame.h * 0.5f }, 
				{ objective->position.x, objective->position.y - objective->current_frame.h * 0.5f });
			App->audio->PlayFx(attack_fx.c_str(), 0, 1);
			break;
		case AttackType::AT_AOE:
		{
			App->audio->PlayFx(explosion_fx.c_str(), 0, 2);
			std::list<Entity*> entities;
			float radius = EXPLOSION_RANGE_TILES * App->map->data.tile_height;
			App->entity_manager->GetEntitiesInArea(radius, objective->position, entities, faction);

			for (std::list<Entity*>::iterator entity = entities.begin(); entity != entities.end(); ++entity)
			{
				(*entity)->DecreaseLife(attack);
			}
			App->particles->CreateParticle(ParticleType::ATTACK_EXPLOSION, objective->position);
		}
			break;
		case AttackType::AT_PIERCING:
			objective->DecreaseLife(attack, true);
			App->particles->CreateParticle(ParticleType::ATTACK_BASIC_SHOT, { position.x, position.y - current_frame.h * 0.5f },
				{ objective->position.x, objective->position.y - objective->current_frame.h * 0.5f });
			App->audio->PlayFx(attack_fx.c_str(), 0, 1);
			break;
		default:
			break;
		}
		attack_timer.Start();
	}
}

SingleUnit* DynamicEntity::GetSingleUnit()
{
	return singleUnit;
}

void DynamicEntity::SetUnitDirection(EntiyDirection unitDirection)
{
}

EntiyDirection DynamicEntity::GetUnitDirection()
{
	return EntiyDirection();
}

void DynamicEntity::SetUnitDirectionByValue(fPoint unitDirection)
{
}

fPoint DynamicEntity::GetUnitDirectionByValue() const
{
	return fPoint();
}
void DynamicEntity::DecreaseLife(float damage, bool piercing)
{
	float defense = entity_card->info.stats.find("defense")->second->GetValue();
	float damage_received = CalculateDamage(damage, defense);

	if (entity_card->info.armored)
	{
		if (piercing)
			damage_received *= 1.25f;
		else
			damage_received *= 0.75f;
	}

	stats.find("health")->second->DecreaseStat(damage_received);
	if (stats.find("health")->second->GetValue() <= 0)
		Die();
}

float DynamicEntity::GetDamage()
{
	return entity_card->info.stats.find("damage")->second->GetValue();
}

bool DynamicEntity::IsArmored()
{
	return entity_card->info.armored;
}

int DynamicEntity::GetAttackType()
{
	return entity_card->info.attack_type;
}

void DynamicEntity::Die()
{
	state = DYNAMIC_DYING;
	current_animation = &animations.find("dying")->second;
	objective = nullptr;
	App->gui->DeleteElement(health_bar);
	path.clear();
}

bool DynamicEntity::CleanUp()
{
	std::map<std::string, Stat*>::iterator item;
	for (item = stats.begin(); item != stats.end(); ++item)
	{
		delete item->second;
		stats.erase(item);
	}
	stats.clear();

	//delete singleUnit;

	return true;
}

void DynamicEntity::AnimationCheck() {
	switch (state)
	{
	case DYNAMIC_IDLE:
		current_animation = &animations.find("idle")->second;
		break;
	case DYNAMIC_MOVING:
		MovementAnimationCheck();
		break;
	case DYNAMIC_ATTACKING:
		AttackingAnimationCheck();
		break;
	case DYNAMIC_DYING:
		current_animation = &animations.find("dying")->second;
		break;
	case DYNAMIC_DEAD:
		current_animation = &animations.find("dead")->second;
		break;
	default:
		break;
	}
}
void DynamicEntity::MovementAnimationCheck() {
	switch (direction)
	{
	case UP:
		current_animation = &animations.find("moving_up")->second;
		break;
	case DOWN:
		current_animation = &animations.find("moving_down")->second;
		break;
	case LEFT:
		current_animation = &animations.find("moving_left")->second;
		break;
	case RIGHT:
		current_animation = &animations.find("moving_right")->second;
		break;
	case UP_RIGHT:
		current_animation = &animations.find("moving_up_right")->second;
		break;
	case UP_LEFT:
		current_animation = &animations.find("moving_up_left")->second;
		break;
	case DOWN_RIGHT:
		current_animation = &animations.find("moving_down_right")->second;
		break;
	case DOWN_LEFT:
		current_animation = &animations.find("moving_down_left")->second;
		break;
	default:
		break;
	}

	current_animation->speed = (entity_card->info.stats.find("movement")->second->GetValue() * current_animation->base_speed) / entity_card->info.stats.find("movement")->second->GetBaseValue();
}
void DynamicEntity::AttackingAnimationCheck() {
	switch (direction)
	{
	case UP:
		current_animation = &animations.find("attacking_up")->second;
		break;
	case DOWN:
		current_animation = &animations.find("attacking_down")->second;
		break;
	case LEFT:
		current_animation = &animations.find("attacking_left")->second;
		break;
	case RIGHT:
		current_animation = &animations.find("attacking_right")->second;
		break;
	case UP_RIGHT:
		current_animation = &animations.find("attacking_up_right")->second;
		break;
	case UP_LEFT:
		current_animation = &animations.find("attacking_up_left")->second;
		break;
	case DOWN_RIGHT:
		current_animation = &animations.find("attacking_down_right")->second;
		break;
	case DOWN_LEFT:
		current_animation = &animations.find("attacking_down_left")->second;
		break;
	default:
		break;
	}

	current_animation->speed = current_animation->GetFrameAmount() * entity_card->info.stats.find("attack_speed")->second->GetValue();
}