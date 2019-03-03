#include "Plansza.h"
#include <cstdlib>
#include <list>
#include <iostream>

const int Plansza::ROZMIAR = 10;

Plansza::Plansza()
{
	NaPowierzchni = 1 * 4 + 2 * 3 + 3 * 2 + 4 * 1;
	std::cout << "Konstruktor klasy Plansza - losowanie polozenia statkow\n";
	bool zajete[10][10] = { false };


	for (int x = 0; x < Plansza::ROZMIAR; x++)
		for (int y = 0; y < Plansza::ROZMIAR; y++)
		{
			komorki[x][y].statek = nullptr;
			komorki[x][y].co = Komorka::NIC;
			komorki[x][y].odkryta = false;
		}


	for (int dlugosc = 4; dlugosc >= 1; dlugosc--)
	{
		int ilosc = 5 - dlugosc;
		for (int i = 0; i < ilosc; i++)
		{
			Statek::Kierunek kierunek = 1.0 * std::rand() / RAND_MAX < 0.5 ? Statek::POZIOMY : Statek::PIONOWY;
			int x, y;

			bool znaleziono = false;

			do
			{
				x = 1.0 * std::rand() / RAND_MAX * (Plansza::ROZMIAR - (kierunek == Statek::POZIOMY ? dlugosc : 0));
				y = 1.0 * std::rand() / RAND_MAX * (Plansza::ROZMIAR - (kierunek == Statek::PIONOWY ? dlugosc : 0));

				znaleziono = true;
				for (int test = 0; test < dlugosc; test++)
				{
					if (kierunek == Statek::POZIOMY && zajete[x + test][y] || kierunek == Statek::PIONOWY && zajete[x][y + test])
					{
						znaleziono = false;
						break;
					}

				}

			} while (!znaleziono);

			Statek *nowy = new Statek(dlugosc, kierunek, x, y);
			komorki[x][y].statek = nowy;
			komorki[x][y].co = Komorka::STATEK;
			zajete[x][y] = true;

			int wys = kierunek == Statek::PIONOWY ? dlugosc : 1;
			int szer = kierunek == Statek::POZIOMY ? dlugosc : 1;
			int x1, y1, x2, y2;
			if (x == 0) x1 = 0; else x1 = x - 1;
			if (x + szer > 9) x2 = 9; else x2 = x + szer;
			if (y == 0) y1 = 0; else y1 = y - 1;
			if (y + wys == 9) y2 = 9; else y2 = y + wys;

			for (int i = x1; i <= x2; i++)
			{
				for (int j = y1; j <= y2; j++)
				{
					zajete[i][j] = true;
				}
			}

			for (int j = 1; j < dlugosc; j++)
			{
				if (kierunek == Statek::POZIOMY)
				{
					komorki[x + j][y].co = Komorka::STATEK;
					komorki[x + j][y].statek = nowy;
					zajete[x + j][y] = true;
				}
				else
				{
					komorki[x][y + j].co = Komorka::STATEK;
					komorki[x][y + j].statek = nowy;
					zajete[x][y + j] = true;
				}
			}
		}
	}

}


Plansza::~Plansza()
{

	for (int x = Plansza::ROZMIAR - 1; x >= 0; x--)
		for (int y = Plansza::ROZMIAR - 1; y >= 0; y--)


			if (komorki[x][y].co != Komorka::NIC && komorki[x][y].statek->x == x && komorki[x][y].statek->y == y)
			{
				delete komorki[x][y].statek;
				komorki[x][y].statek = nullptr;
			}
}

void Plansza::Trafiony(int xp, int yp)

{
	if (komorki[xp][yp].co == Komorka::STATEK)
	{
		NaPowierzchni--;
		komorki[xp][yp].co = Komorka::STATEK_TRAFIONY;

		bool zatopiony = true;
		Statek *s = komorki[xp][yp].statek;
		for (int c = 0; c < s->dlugosc; c++)
			if (s->kierunek == Statek::POZIOMY && komorki[s->x + c][s->y].co != Komorka::STATEK_TRAFIONY)
				zatopiony = false;
			else
				if (s->kierunek == Statek::PIONOWY && komorki[s->x][s->y + c].co != Komorka::STATEK_TRAFIONY)
					zatopiony = false;

		s->zatopiony = zatopiony;
	}
}
