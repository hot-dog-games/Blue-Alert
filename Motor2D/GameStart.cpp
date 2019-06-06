#include "j1App.h"
#include "Video.h"
#include "SceneManager.h"
#include "GameStart.h"



GameStart::GameStart()
{
}


GameStart::~GameStart()
{
}

bool GameStart::Start()
{
	App->video->PlayVideo("videos/hotdogpresents.mp4");
	return true;
}

bool GameStart::Update(float dt)
{
	if (!App->video->playing)
	{
		App->scene_manager->ChangeScene(SceneType::MENU);
	}
	return true;
}

bool GameStart::CleanUp()
{
	return true;
}
