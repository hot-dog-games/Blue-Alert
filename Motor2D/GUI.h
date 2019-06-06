#ifndef __GUI_H__
#define __GUI_H__

#include "Module.h"
#include "p2Point.h"

#define CURSOR_WIDTH 2

struct SDL_Rect;
struct SDL_Color;
struct SDL_Texture;

struct _TTF_Font;
class UIElement;
class UIImage;
class UIButton;
class UISelectableButton;
class UIButtonText;
class UIButtonTroops;
class UIScrollBar;
class UIAnimatedImage;
class UILabel;
class UIBar;
class UIPopUp;
class Stat;
class Entity;

enum GUI_Event {
	LEFT_CLICK_DOWN,
	LEFT_CLICK_UP,
	RIGHT_CLICK_DOWN,
	RIGHT_CLICK_UP,
	MOUSE_OVER,
	MOUSE_EXIT
};

enum ScrollType {
	VERTICAL,
	HORIZONTAL
};

enum BarType {
	BAR_NONE = -1,
	BAR_VERTICAL,
	BAR_HORITZONTAL
};

enum BarState {
	BAR_STATIC,
	BAR_DYNAMIC
};

enum SliderType {
	FX,
	MUSIC
};

enum ButtonLevel {
	LVL_1,
	LVL_2,
	LVL_3,
	LVL_4
};

// ---------------------------------------------------
class Gui : public Module
{
public:

	Gui();

	// Destructor
	virtual ~Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions
	UIImage* CreateImage(iPoint pos, SDL_Rect rect, UIElement* parent = nullptr, bool image = true);
	UILabel* CreateLabel(iPoint pos, std::string path, int size, std::string text, SDL_Color color, int max_width = 0, UIElement* parent = nullptr, bool is_interactable = false);
	UIButton* CreateButton(iPoint pos, SDL_Rect* sprite_rect, UIElement* parent = nullptr, bool is_interactable = true);
	UISelectableButton* CreateSelectableButton(iPoint pos, SDL_Rect* sprite_rect, UIElement* parent = nullptr, bool is_interactable = true);
	UIButtonText* CreateButtonText(iPoint pos, iPoint offset, SDL_Rect* sprite_rect, std::string text, SDL_Color color, int size = 20, UIElement* parent = nullptr, bool is_interactable = true);
	UIButtonTroops* CreateButtonTroops(iPoint pos, SDL_Rect* sprite_rect, ButtonLevel lvl, UIElement* parent = nullptr, bool is_interactable = true);
	UIScrollBar* CreateScrollBar(iPoint pos, SDL_Rect rect, SliderType type, int initial_value, int max_value, UIElement* parent = nullptr);
	UIAnimatedImage* CreateAnimatedImage(iPoint pos, SDL_Rect * rect, int total_sprites, int speed, UIElement* parent = nullptr);
	UIBar* CreateBar(iPoint pos, SDL_Rect rect, Stat* value, BarType type = BarType::BAR_VERTICAL, BarState state = BarState::BAR_DYNAMIC, Entity* entity= nullptr, UIElement* parent = nullptr, bool color_change = false);
	UIPopUp* CreatePopUp(SDL_Rect rect, iPoint margin, std::string text, int text_size, SDL_Color color,  UIElement* parent = nullptr);

	void SliderAction(SliderType type, UIScrollBar* slider);

	void DeleteElement(UIElement* element);
	void DisableUI();
	UIElement* GetElementUnderMouse();
	void EnableElement(UIElement* element);
	void DisableElement(UIElement* element);

	void EnableInteractable(UIElement * ele);
	void DisableInteractable(UIElement * ele);

	void RenderElement(UIElement* element);

	SDL_Rect* LoadUIButton(int num, std::string type);
	SDL_Rect  LoadUIImage(int num, std::string type);

	SDL_Texture* GetAtlas() const;
	_TTF_Font*	 GetButtonFont() const;

	bool popup_active = false;

private:
	std::list<UIElement*> elements;
	SDL_Texture* atlas;
	std::string atlas_file_name;
	pugi::xml_document buttons_file;

	//scaling

	bool debug_draw = false;

	bool is_world_ui = false;

	_TTF_Font* button_font = nullptr;

};

#endif // __j1GUI_H__
