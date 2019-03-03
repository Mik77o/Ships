#pragma once

#include "Statek.h"

class Plansza
{
public:
	Plansza();
	~Plansza();


	struct Komorka
	{
		enum
		{
			NIC,
			STATEK,
			STATEK_TRAFIONY,
		} co;

		Statek *statek;
		bool odkryta;
	};




	bool CzyZajety(int x, int y)
	{
		return komorki[x][y].co != Komorka::NIC;
	}


	bool CzyTrafiony(int x, int y)
	{
		return komorki[x][y].co == Komorka::STATEK_TRAFIONY;
	}


	void Trafiony(int xp, int yp);

	static const int ROZMIAR;
	unsigned int NaPowierzchni;
	Komorka komorki[10][10];
};
