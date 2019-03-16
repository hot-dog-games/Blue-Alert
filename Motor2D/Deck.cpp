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
	LOG("Deck cleanup");
	for (int i = 0; i < MAX_CARDS; ++i)
	{
		if (cards[i])
		{
			App->tex->UnLoad(cards[i]->sprite_path);
			if(delete_cards)
				cards[i]->to_delete = true;
		}
	}
	delete[] cards;
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
