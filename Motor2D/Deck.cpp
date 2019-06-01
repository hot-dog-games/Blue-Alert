#include "p2Log.h"
#include "j1App.h"
#include "Textures.h"
#include "CardManager.h"
#include "Deck.h"


Deck::Deck()
{
	for (int i = 0; i < MAX_CARDS; ++i)
	{
		cards[i] = nullptr;
	}
}


Deck::~Deck()
{
}

void Deck::CleanUp()
{
	LOG("DECK CLEANUP");
	for (int i = 0; i < MAX_CARDS; ++i)
	{
		if (cards[i])
		{
			App->tex->UnLoad(cards[i]->sprite_path);
			if (delete_cards)
				App->card_manager->DeleteCard(cards[i]);
		}
	}
}

void Deck::AddCard(Card* card)
{
	for (int i = 0; i < MAX_CARDS; ++i)
	{
		if (!cards[i])
		{
			cards[i] = card;
			break;
		}
	}
}

void Deck::RemoveCard(uint position)
{
	cards[position] = nullptr;
}

Card * Deck::GetCard(EntityType type)
{
	for (int i = 0; i < MAX_CARDS; i++) {
		if (cards[i] && cards[i]->type == type) 
			return cards[i];
		
	}

	return nullptr;
}

int Deck::GetDeckSize() const
{
	int num = 0;

	for (int i = 0; i < MAX_CARDS; i++) {
		if (cards[i])
			num++;
	}

	return num;
}
