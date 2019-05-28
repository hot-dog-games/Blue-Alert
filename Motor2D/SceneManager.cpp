#include "j1App.h"
#include "EntityManager.h"
#include "GUI.h"
#include "TestingScene.h"
#include "StrategyMapScene.h"
#include "Particles.h"
#include "Audio.h"
#include "Fonts.h"
#include "BattleScene.h"
#include "MainMenuScene.h"
#include "Map.h"
#include "SceneManager.h"



SceneManager::SceneManager()
{
	name = "scene_manager";
}


SceneManager::~SceneManager()
{

}

bool SceneManager::Awake(pugi::xml_node &)
{
	current_scene = new MainMenuScene();
	return true;
}

bool SceneManager::Start()
{
	current_scene->Start();
	return true;
}

bool SceneManager::PreUpdate()
{
	current_scene->PreUpdate();
	return true;
}

bool SceneManager::Update(float dt)
{
	current_scene->Update(dt);
	return true;
}

bool SceneManager::PostUpdate()
{
	bool ret = current_scene->PostUpdate();
	return ret;
}

bool SceneManager::CleanUp()
{
	current_scene->CleanUp();
	return true;
}

bool SceneManager::Load(pugi::xml_node &xml)
{
	current_scene->Load(xml);
	return true;
}

bool SceneManager::Save(pugi::xml_node &xml) const
{
	current_scene->Save(xml);
	return true;
}

bool SceneManager::Pause()
{
	if (current_scene)
		current_scene->Pause();
	return true;
}

bool SceneManager::Resume()
{
	if (current_scene)
		current_scene->Resume();
	return true;
}

//Scene unloads current scene, then creates and loads new scene.
void SceneManager::ChangeScene(int new_scene)
{
	current_scene->CleanUp();
	App->entity_manager->CleanUp();
	App->particles->CleanUp();
	App->gui->CleanUp();
	App->map->CleanUp();

	delete current_scene;
	current_scene = nullptr;

	switch (new_scene)
	{
	case MENU:
		current_scene = new MainMenuScene();
		break;
	case MAP:
		current_scene = new StrategyMapScene();
		break;
	case COMBAT:
		current_scene = new BattleScene();
		break;
	case TESTING:
		current_scene = new TestingScene();
		break;
	}

	current_scene->Start();
}