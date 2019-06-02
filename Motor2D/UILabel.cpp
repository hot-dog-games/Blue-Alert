#include "j1App.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "p2Log.h"
#include "UILabel.h"

UILabel::UILabel(iPoint pos, _TTF_Font* font, std::string text, SDL_Color color, int max_width, bool is_interactable)
{
	interactable = is_interactable;

	int w, h;
	App->fonts->CalcSize("a", w, h, font); //calc size of one char

	if (max_width > 0)
	{
		//calculate the amount of lines that string has relative to the max width
		int lines = 1;
		int char_in_line = 0;
		int max_chars_in_line = max_width / w;
		for (int i = 0; i < text.size(); i++)
		{
			if (text.c_str()[i] == '\n')
			{
				lines++;
				char_in_line = 0;
			}
			else if (char_in_line > max_chars_in_line)
			{
				lines++;
				char_in_line = 0;
			}
			char_in_line++;
		}

		rect_box.w = max_width;
		rect_box.h = h * lines;
	}
	else
	{
		//calc amount of lines independent of a max width
		int lines = 1;
		int max_chars_in_line = 0;
		int chars_in_line = 0;
		for (int i = 0; i < text.size(); i++)
		{
			if (text.c_str()[i] == '\n')
			{
				lines++;
				max_chars_in_line = chars_in_line > max_chars_in_line ? chars_in_line : max_chars_in_line;
				chars_in_line = 0;
			}
			chars_in_line++;
		}
		max_chars_in_line = chars_in_line > max_chars_in_line ? chars_in_line : max_chars_in_line;

		//adjust size of box to the chars and lines
		rect_box.w = w * max_chars_in_line;
		rect_box.h = h * lines;
	}

	rect_box.x = pos.x;
	rect_box.y = pos.y;
	w = rect_box.w;
	h = rect_box.h;

	this->font = font;
	this->text = text;
	this->color = color;
	this->original_color = color;
	this->centered = centered;
	texture = App->fonts->Print(this->text.c_str(), color, font, rect_box.w);
}
UILabel::~UILabel()
{
}

bool UILabel::UIBlit()
{
	iPoint screen_pos = GetScreenPos();

	if (clipping && parent)
		App->render->Blit(texture, screen_pos.x, screen_pos.y, nullptr, 0.0F, 0.0, INT_MAX, INT_MAX, 1.0F, 1.0F, &parent->GetScreenRect());
	else
		App->render->Blit(texture, screen_pos.x, screen_pos.y, nullptr, 0.0F, 0.0, INT_MAX, INT_MAX);


	return true;
}

void UILabel::SetText(std::string text)
{
	if (text != this->text)
	{
		LOG("unload label text");
		App->tex->UnLoad(texture);
		this->text = text;
		texture = App->fonts->Print(this->text.c_str(), color, font, rect_box.w);

		if (centered)
		{
			Center();
		}

	}
}

void UILabel::SetColor(SDL_Color color)
{
	this->color = color;
	this->original_color = color;
}

bool UILabel::CleanUp()
{
	LOG("label cleanup text");
	parent = nullptr;
	font = nullptr;
	App->tex->UnLoad(texture);

	return true;
}

void UILabel::OnMouseClick()
{
	color = { 100,100,100,255 };
}

void UILabel::OnMouseHover()
{
	color = { 255,255,255,255 };
}

void UILabel::OnMouseRelease()
{
	color = { 255,255,255,255 };
}

void UILabel::OnMouseExit()
{
	color = original_color;
}

void UILabel::SetCentered(bool value)
{
	centered = value;
	if(centered)
		Center();
}

void UILabel::Center()
{
	int w, h;
	App->fonts->CalcSize(this->text.c_str(), w, h, font);
	rect_box.x = (parent->GetLocalRect().w * 0.5f) - (w * 0.5f);
	rect_box.y = (parent->GetLocalRect().h * 0.5f) - (h * 0.5f);
}
