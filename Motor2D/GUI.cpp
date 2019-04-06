#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "SceneManager.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "UIElement.h"
#include "UIAnimatedImage.h"
#include "UIImage.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UIScrollBar.h"
#include "UIBar.h"
#include "GUI.h"
#include "Stat.h"

Gui::Gui() : Module()
{
	name = "gui";
}

// Destructor
Gui::~Gui()
{
	App->tex->UnLoad(atlas);
	atlas = nullptr;
}

// Called before render is available
bool Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool Gui::Start()
{
	atlas = App->tex->Load("ui/atlas.png");

	return true;
}

// Update all guis
bool Gui::PreUpdate()
{
	//BROFILER_CATEGORY("UIPreUpdate", Profiler::Color::Magenta);
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug_draw = !debug_draw;


	UIElement* selected_element = GetElementUnderMouse();

	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		UIElement* current_element = (*element);
		if (selected_element && selected_element == current_element)
		{
			if (!current_element->hovered)
			{
				current_element->OnMouseHover();
				App->scene_manager->current_scene->GUIEvent(current_element, MOUSE_OVER);
				current_element->hovered = true;
			}
		}
		if (current_element->hovered)
		{
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				current_element->OnMouseClick();
				App->scene_manager->current_scene->GUIEvent(current_element, LEFT_CLICK_DOWN);
				current_element->clicked = true;
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				current_element->OnMouseRelease();
				App->scene_manager->current_scene->GUIEvent(current_element, LEFT_CLICK_UP);
				current_element->clicked = false;
			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			{
				//drag
				if (current_element->dragable)
				{
					iPoint mouse_pos;
					App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);
					current_element->SetScreenPos(mouse_pos.x - current_element->GetLocalRect().w / 2, mouse_pos.y - current_element->GetLocalRect().h / 2);

					if (current_element->parent_limit && current_element->parent)
					{
						SDL_Rect element_rect = current_element->GetScreenRect();
						SDL_Rect parent_rect = current_element->parent->GetScreenRect();

						if (element_rect.x < parent_rect.x)
							element_rect.x = parent_rect.x;
						else if (element_rect.x + element_rect.w > parent_rect.x + parent_rect.w)
							element_rect.x = (parent_rect.x + parent_rect.w) - element_rect.w;
						if (element_rect.y < parent_rect.y)
							element_rect.y = parent_rect.y;
						else if (element_rect.y + element_rect.h > parent_rect.y + parent_rect.h)
							element_rect.y = (parent_rect.y + parent_rect.h) - element_rect.h;

						current_element->SetScreenPos(element_rect.x, element_rect.y);
					}
				}
			}
			if (!current_element->clicked && selected_element != current_element)
			{
				current_element->OnMouseExit();
				App->scene_manager->current_scene->GUIEvent(current_element, MOUSE_EXIT);
				current_element->hovered = false;
			}
		}
		
	}
	return true;
}

bool Gui::Update(float dt)
{
	//scale stuff
	
	//BROFILER_CATEGORY("UIUpdate", Profiler::Color::Magenta);
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->enabled)
		{
			(*element)->Update(dt);
		}

	}
	return true;
}

// Called after all Updates
bool Gui::PostUpdate()
{
	//BROFILER_CATEGORY("UIPostUpdate", Profiler::Color::Magenta);
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->enabled)
		{
			(*element)->UIBlit();
			if (debug_draw)
				App->render->DrawQuad((*element)->GetScreenRect(), 255, 0, 0, 255, false, false);
		}

	}
	return true;
}

// Called before quitting
bool Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		(*element)->CleanUp();
	}
	elements.clear();

	return true;
}

UIImage* Gui::CreateImage(iPoint pos, SDL_Rect rect, UIElement* parent, bool image_)
{
	UIImage* image = new UIImage(pos, rect, image_);
	image->parent = parent;
	elements.push_back(image);

	return image;
}

UILabel* Gui::CreateLabel(iPoint pos, std::string path, int size, std::string text, SDL_Color color, int max_width, UIElement* parent)
{
	_TTF_Font* font = App->fonts->Load(path.c_str(), size);
	UILabel* label = new UILabel(pos, font, text, color, max_width);
	label->parent = parent;
	elements.push_back(label);

	return label;
}

UIButton* Gui::CreateButton(iPoint pos, SDL_Rect* sprite_rect, UIElement* parent, bool is_interactable)
{
	UIButton* button = new UIButton(pos, sprite_rect, is_interactable);
	button->parent = parent;
	elements.push_back(button);

	return button;
}

UIScrollBar* Gui::CreateScrollBar(iPoint pos, float min, float max, ScrollType type, UIElement* parent)
{
	UIScrollBar* scroll = new UIScrollBar(pos, type);
	scroll->parent = parent;
	scroll->SetMinMax(min, max);
	elements.push_back(scroll);

	return scroll;
}

UIAnimatedImage* Gui::CreateAnimatedImage(iPoint pos, SDL_Rect * rect, int total_sprites, int speed, UIElement* parent)
{
	UIAnimatedImage* image = new UIAnimatedImage(pos, rect, total_sprites, speed);
	image->parent = parent;
	elements.push_back(image);
	return image;
}

UIBar * Gui::CreateBar(iPoint pos, SDL_Rect rect, Stat* value, UIElement * parent)
{
	UIBar* bar = new UIBar(pos, rect, value, parent);
	bar->parent = parent;
	elements.push_back(bar);
	return bar;
}

void Gui::DeleteElement(UIElement* ele)
{
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->parent && (*element)->parent == ele)
			DeleteElement(*element);
	}
	ele->CleanUp();
	elements.remove(ele);
	delete ele;
}

void Gui::EnableElement(UIElement* ele)
{
	ele->SetEnabled(true);
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->parent && (*element)->parent == ele)
			EnableElement(*element);
	}
}
void Gui::DisableElement(UIElement* ele)
{
	ele->SetEnabled(false);
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->parent && (*element)->parent == ele)
			DisableElement(*element);
	}
}
UIElement* Gui::GetElementUnderMouse()
{
	int x, y;
	App->input->GetMousePosition(x, y);

	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->IsInside(x, y) && (*element)->interactable && (*element)->enabled)
		{
			bool inside_child = false;
			for (std::list<UIElement*>::iterator child_item = elements.begin(); child_item != elements.end(); child_item = ++child_item)
			{
				if ((*child_item)->parent && (*child_item)->parent == (*element) && (*child_item)->IsInside(x, y) && (*child_item)->interactable && (*element)->enabled)
				{
					inside_child = true;
					break;
				}
			}
			if (!inside_child)
				return (*element);
		}
	}

	return nullptr;
}



// const getter for atlas
SDL_Texture* Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------