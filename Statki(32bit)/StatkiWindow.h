#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Plansza.h"
#include "Gra.h"
#include <string>
#include "Timer.h"
#include <list>
#include <utility>

typedef std::pair<int, int> Polozenie;

enum
	STAN_GRY
{
	W_TRAKCIE,
	WYGRAL_KOMPUTER,
	WYGRAL_GRACZ
};

class StatkiWindow : public sf::RenderWindow
{
public:
	enum Tryb
	{
		GRACZ,
		KOMPUTER,
	};

	StatkiWindow(Plansza *plansza, Tryb tryb);
	~StatkiWindow();
	void UstawOponenta(StatkiWindow *oponent);

	void tick(Tryb &czyjRuch);
	void ustawStatus(std::string s)
	{
		status = s;
		tmStatus.Reset();
		tmStatus.Ustaw(1);
	}

private:
	STAN_GRY stanGry;
	Tryb tryb;
	Plansza *plansza;
	StatkiWindow *oponent;
	Timer tmStatus;

	sf::Font font;

	std::string status;
	std::string NazwaKomorki(int x, int y);


	std::list<Polozenie> ruchyKomputera;
	bool pamiec[10][10];
	Timer tmRuchKomputera;
	int kierunek;
};

