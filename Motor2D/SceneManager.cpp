#include "TestingScene.h"
<<<<<<< HEAD
#include "MapMenuScene.h"
=======
#include "StrategyMap.h"
>>>>>>> parent of ee7c7ac... Revert "Merge branch 'develop' into PanelUI"
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
<<<<<<< HEAD
	current_scene = new MapMenuScene();
=======
	current_scene = new StrategyMap();
>>>>>>> parent of ee7c7ac... Revert "Merge branch 'develop' into PanelUI"
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
	current_scene->PostUpdate();
	return true;
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

//Scene unloads current scene, then creates and loads new scene.
void SceneManager::ChangeScene(int new_scene)
{
	current_scene->CleanUp();
	delete current_scene;
	current_scene = nullptr;

	switch (new_scene)
	{
	case MENU:
		break;
	case MAP:
		current_scene = new StrategyMap();
		break;
	case COMBAT:
		break;
	case TESTING:
		current_scene = new TestingScene();
		break;
	}

	current_scene->Start();
}
