#include "j1App.h"
#include "Render.h"
#include "Video.h"
#include "SceneManager.h"
#include "Input.h"
#include "GameStart.h"



GameStart::GameStart()
{
}


GameStart::~GameStart()
{
}

bool GameStart::Start()
{
	App->video->PlayVideo("videos/cinematic_trailer.mp4");
	return true;
}

bool GameStart::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->video->CloseVideo();

	if (!App->video->playing)
		App->scene_manager->ChangeScene(SceneType::MENU);

	return true;
}

bool GameStart::CleanUp()
{
	return true;
}
