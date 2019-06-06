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
#include "UISelectableButton.h"
#include "UIButtonText.h"
#include "UIButtonTroops.h"
#include "UILabel.h"
#include "UIScrollBar.h"
#include "UIBar.h"
#include "UIPopUp.h"
#include "UIEntityBar.h"
#include "Brofiler/Brofiler.h"
#include "Stat.h"
#include "GUI.h"


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

	buttons_file.load_file("xml/ui.xml");

	button_font = App->fonts->Load("fonts/button_text.ttf", 12);

	App->audio->LoadFx("audio/fx/Ambient_Sounds/Shots/One_shoot.1.wav");

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
	BROFILER_CATEGORY("UIPreUpdate", Profiler::Color::Magenta);
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
	
	BROFILER_CATEGORY("UIUpdate", Profiler::Color::Magenta);
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
	BROFILER_CATEGORY("UIPostUpdate", Profiler::Color::Magenta);
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->enabled && !(*element)->parent)
		{
			RenderElement((*element));
		}

	}
	return true;
}

// Called before quitting
bool Gui::CleanUp()
{
	LOG("Freeing GUI");
	while (!elements.empty())
	{
		elements.front()->CleanUp();
		delete elements.front();
		elements.pop_front();
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

UILabel* Gui::CreateLabel(iPoint pos, std::string path, int size, std::string text, SDL_Color color, int max_width, UIElement* parent, bool is_interactable)
{
	_TTF_Font* font = App->fonts->Load(path.c_str(), size);
	UILabel* label = new UILabel(pos, font, text, color, max_width, is_interactable);
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

UISelectableButton * Gui::CreateSelectableButton(iPoint pos, SDL_Rect * sprite_rect, UIElement * parent, bool is_interactable)
{
	UISelectableButton* button = new UISelectableButton(pos, sprite_rect, is_interactable);
	button->parent = parent;
	elements.push_back(button);

	return button;
}

UIButtonText * Gui::CreateButtonText(iPoint pos, iPoint offset, SDL_Rect * sprite_rect, std::string text, SDL_Color color, int size, UIElement* parent, bool is_interactable)
{
	UIButtonText* button = new UIButtonText(pos, offset, sprite_rect, text, color, size, is_interactable);
	button->parent = parent;
	elements.push_back(button);

	return button;
}

UIButtonTroops* Gui::CreateButtonTroops(iPoint pos, SDL_Rect* sprite_rect, ButtonLevel lvl, UIElement* parent, bool is_interactable) {

	UIButtonTroops* button = new UIButtonTroops(pos, sprite_rect, is_interactable, lvl);
	button->parent = parent;
	elements.push_back(button);

	return button;
}

UIScrollBar* Gui::CreateScrollBar(iPoint pos, SDL_Rect rect, SliderType type, int initial_value, int max_value, UIElement* parent)
{
	UIScrollBar* scroll = new UIScrollBar(pos, rect, type, initial_value, max_value);
	scroll->parent = parent;
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

UIBar * Gui::CreateBar(iPoint pos, SDL_Rect rect, Stat* value, BarType type, BarState state, Entity* entity, UIElement * parent, bool color_change)
{
	UIBar* bar;
	if (entity) {
		bar = new UIEntityBar(pos, rect, value, type, state, entity, color_change);
	}
	else {
		bar = new UIBar(pos, rect, value, type, state, color_change);
	}
	
	bar->parent = parent;
	elements.push_front(bar);
	return bar;
}

UIPopUp * Gui::CreatePopUp(SDL_Rect rect, iPoint margin, std::string text, int text_size, SDL_Color color, UIElement* parent)
{
	UIPopUp* pop_up = new UIPopUp(rect, margin, text, text_size, color);

	pop_up->parent = parent;
	elements.push_front(pop_up);
	return pop_up;
}

void Gui::SliderAction(SliderType type, UIScrollBar * slider)
{
	if (type == MUSIC) {
		App->audio->SetMusicVolume(slider->current_value);
	}
	if (type == FX) {
		App->audio->SetAllFXVolume(slider->current_value);
		App->audio->PlayFx("audio/fx/Ambient_Sounds/Shots/One_shoot.1.wav", 0, 0);
	}
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

void Gui::DisableUI()
{
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if((*element)->enabled)
			DisableElement((*element));
	}
}

void Gui::EnableElement(UIElement* ele)
{
	if (ele) {
		ele->SetEnabled(true);
		for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
		{
			if ((*element)->parent && (*element)->parent == ele)
				EnableElement(*element);
		}
	}
}

void Gui::DisableElement(UIElement* ele)
{
	if (ele) {
		ele->SetEnabled(false);
		for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
		{
			if ((*element)->parent && (*element)->parent == ele)
				DisableElement(*element);
		}
	}
}

void Gui::EnableInteractable(UIElement* ele)
{
	ele->interactable = true;
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->parent && (*element)->parent == ele)
			(*element)->interactable = true;
	}
}

void Gui::DisableInteractable(UIElement* ele)
{
	ele->interactable = false;
	for (std::list<UIElement*>::iterator element = elements.begin(); element != elements.end(); ++element)
	{
		if ((*element)->parent && (*element)->parent == ele)
			(*element)->interactable = false;
	}
}

void Gui::RenderElement(UIElement* element)
{
	element->UIBlit();
	if (debug_draw)
		App->render->DrawQuad(element->GetScreenRect(), 255, 0, 0, 255, false, false);

	for (std::list<UIElement*>::iterator child_element = elements.begin(); child_element != elements.end(); ++child_element)
	{
		if ((*child_element)->enabled && (*child_element)->parent && (*child_element)->parent == element)
			RenderElement((*child_element));
	}
}

SDL_Rect* Gui::LoadUIButton(int num, std::string type)
{
	pugi::xml_node buttons_node = buttons_file.first_child().child("ui_button");

	std::string name;
	int num_type = 0;

	for (pugi::xml_node it = buttons_node.first_child(); it; it = it.next_sibling())
	{
		num_type = it.first_attribute().as_int();
		if (num_type == num)
		{
			name = it.name();
			break;
		}
	}

	SDL_Rect* button_rect = new SDL_Rect[4];
	int anim_num = 0;

	for (pugi::xml_node it_node = buttons_node.child(name.c_str()).child(type.c_str()).child("frame"); it_node; it_node = it_node.next_sibling()) {
		button_rect[anim_num].x = it_node.attribute("x").as_uint();
		button_rect[anim_num].y = it_node.attribute("y").as_uint();
		button_rect[anim_num].w = it_node.attribute("width").as_uint();
		button_rect[anim_num].h = it_node.attribute("height").as_uint();

		anim_num++;
	}

	return button_rect;
}

SDL_Rect Gui::LoadUIImage(int num, std::string type)
{
	pugi::xml_node images_node = buttons_file.first_child().child("ui_image");

	std::string name;

	switch (num) {
	case 30:
		name = "Infantry";
		break;
	case 31:
		name = "Aerial";
		break;
	case 32:
		name = "Land";
		break;
	case 33:
		name = "Gold";
		break;
	case 34:
		name = "Store";
		break;
	}

	SDL_Rect image_rect;
	pugi::xml_node rect_node = images_node.child(name.c_str()).child(type.c_str());

	image_rect.x = rect_node.attribute("x").as_uint();
	image_rect.y = rect_node.attribute("y").as_uint();
	image_rect.w = rect_node.attribute("width").as_uint();
	image_rect.h = rect_node.attribute("height").as_uint();

	return image_rect;
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
				if ((*child_item)->parent && (*child_item)->parent == (*element) && (*child_item)->IsInside(x, y) && (*child_item)->interactable && (*child_item)->enabled)
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

//void Gui::LoadUI(std::string xml_path)
//{
//	pugi::xml_document ui_file;
//	ui_file.load_file(xml_path.c_str());
//	pugi::xml_node ui_node = ui_file.first_child();
//
//	for (pugi::xml_node ui_element_node = ui_node.child("element"); ui_element_node; ui_element_node = ui_element_node.next_sibling())
//	{
//		std::string type = ui_element_node.attribute("type").as_string();
//		std::string name = ui_element_node.attribute("name").as_string();
//
//		UIElement* ui_element = nullptr;
//		pugi::xml_node ui_element_attributes = ui_element_node.first_child();
//
//		if (type == "button")
//		{
//			SDL_Rect sprite_rect[4];
//			ui_element = App->gui->CreateButton({
//				ui_element_attributes.attribute("pos_x").as_uint(),
//				ui_element_attributes.attribute("pos_y").as_uint()
//				},
//				)
//		}
//		else if (type == "label")
//		{
//
//		}
//		else if (type == "image")
//		{
//
//		}
//		else if (type == "bar")
//		{
//
//		}
//
//		if (ui_element != nullptr)
//			ui_elements.insert(std::pair<std::string, UIElement*>(name, ui_element));
//	}
//}



// const getter for atlas
SDL_Texture* Gui::GetAtlas() const
{
	return atlas;
}

_TTF_Font * Gui::GetButtonFont() const
{
	return button_font;
}


// class Gui ---------------------------------------------------