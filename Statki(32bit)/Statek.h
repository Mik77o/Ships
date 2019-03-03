#pragma once

#include <SFML/Graphics/Color.hpp>


class Statek
{
public:
	int dlugosc;
	int x, y;
	enum Kierunek
	{
		POZIOMY,
		PIONOWY
	};
	Kierunek kierunek;
	sf::Color kolor;
	bool zatopiony;

	Statek(int dlugosc, Kierunek kierunek, int x, int y);
	~Statek();
};


