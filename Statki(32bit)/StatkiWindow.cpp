#include "StatkiWindow.h"
#include <cstdio>
#include <iostream>
#include <list>


static const int ROZMIAR_MASZTU = 50;

StatkiWindow::StatkiWindow(Plansza *plansza, Tryb tryb) : sf::RenderWindow(sf::VideoMode(500, 525), tryb == GRACZ ? "PLANSZA GRACZA" : "PLANSZA KOMPUTERA"),
tryb(tryb), plansza(plansza)
{

	if (!font.loadFromFile("LiberationSans-Bold.ttf"))
		std::cerr << "Nie udalo sie zaladowac czcionki\n";

	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 10; y++)
			pamiec[x][y] = false;
	stanGry = W_TRAKCIE;
}

StatkiWindow::~StatkiWindow()
{
}

std::string StatkiWindow::NazwaKomorki(int x, int y)
{

	std::string nazwa("");
	nazwa += (char)('A' + x);
	nazwa += std::to_string(y + 1);
	return nazwa;
}

void StatkiWindow::UstawOponenta(StatkiWindow *op)
{

	oponent = op;
}

void StatkiWindow::tick(Tryb &czyjRuch)
{
	sf::Event event;


	while (pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			close();


		if (
			stanGry == W_TRAKCIE && event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left &&
			tryb == KOMPUTER && czyjRuch == GRACZ
			)
		{


			int xp = event.mouseButton.x / ROZMIAR_MASZTU % 10;
			int yp = event.mouseButton.y / ROZMIAR_MASZTU % 10;


			if (!plansza->komorki[xp][yp].odkryta)
			{

				if (plansza->CzyZajety(xp, yp) && !plansza->CzyTrafiony(xp, yp))
				{

					plansza->Trafiony(xp, yp);
					ustawStatus(std::string("Trafiony: ") + NazwaKomorki(xp, yp));


					if (!plansza->NaPowierzchni)
					{
						stanGry = WYGRAL_GRACZ;

						oponent->stanGry = WYGRAL_GRACZ;
					}
				}
				else
				{

					plansza->komorki[xp][yp].odkryta = true;
					ustawStatus("Pudlo");
					czyjRuch = KOMPUTER;


					oponent->tmRuchKomputera.Ustaw(1);
				}
			}
		}
	}


	clear(sf::Color::White);

	for (int i = 0; i < 10; i++)
	{
		sf::RectangleShape liniaPozioma(sf::Vector2f(1, ROZMIAR_MASZTU * 10));
		sf::RectangleShape liniaPionowa(sf::Vector2f(ROZMIAR_MASZTU * 10, 1));
		liniaPozioma.setFillColor(sf::Color::Black);
		liniaPozioma.setPosition(ROZMIAR_MASZTU * (i + 1), 0);
		liniaPionowa.setFillColor(sf::Color::Black);
		liniaPionowa.setPosition(0, ROZMIAR_MASZTU * (i + 1));
		draw(liniaPionowa);
		draw(liniaPozioma);
	}

	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 10; y++)
			if (plansza->komorki[x][y].statek != nullptr && (tryb == GRACZ || plansza->CzyTrafiony(x, y))) {
				Statek *s = plansza->komorki[x][y].statek;
				sf::RectangleShape statek(sf::Vector2f(
					ROZMIAR_MASZTU,
					ROZMIAR_MASZTU
				));
				statek.setFillColor(plansza->komorki[x][y].statek->kolor);
				statek.setPosition(ROZMIAR_MASZTU * x, ROZMIAR_MASZTU * y);
				draw(statek);
				if (plansza->CzyTrafiony(x, y))
				{
					sf::CircleShape traf;
					traf.setFillColor(sf::Color::White);
					traf.setOutlineColor(sf::Color::Black);
					traf.setOutlineThickness(2);
					traf.setRadius(10);
					traf.setPosition(ROZMIAR_MASZTU * x + 15, ROZMIAR_MASZTU * y + 15);
					draw(traf);
				}
				if (s->zatopiony) {
					sf::RectangleShape bulbul(sf::Vector2f((s->kierunek == Statek::POZIOMY ? ROZMIAR_MASZTU : 2),
						(s->kierunek == Statek::PIONOWY ? ROZMIAR_MASZTU : 2)));
					bulbul.setPosition(ROZMIAR_MASZTU * x + (s->kierunek == Statek::PIONOWY ? 25 : 0), ROZMIAR_MASZTU * y + (s->kierunek == Statek::POZIOMY ? 25 : 0));
					bulbul.setFillColor(sf::Color::Black);
					draw(bulbul);
				}

			}
			else if (tryb == KOMPUTER && plansza->komorki[x][y].co == Plansza::Komorka::NIC && plansza->komorki[x][y].odkryta)
			{
				sf::CircleShape pudlo;
				pudlo.setOutlineColor(sf::Color(255, 194, 192));
				pudlo.setFillColor(sf::Color(224, 32, 32));
				pudlo.setOutlineThickness(3);
				pudlo.setRadius(6);
				pudlo.setPosition(ROZMIAR_MASZTU * x + 20, ROZMIAR_MASZTU * y + 20);
				draw(pudlo);
			}
			else if (tryb == GRACZ && plansza->komorki[x][y].co == Plansza::Komorka::NIC && plansza->komorki[x][y].odkryta)
			{
				sf::CircleShape pudlo_komp;
				pudlo_komp.setOutlineColor(sf::Color(100, 100, 100));
				pudlo_komp.setFillColor(sf::Color(1, 1, 1));
				pudlo_komp.setOutlineThickness(3);
				pudlo_komp.setRadius(6);
				pudlo_komp.setPosition(ROZMIAR_MASZTU * x + 20, ROZMIAR_MASZTU * y + 20);
				draw(pudlo_komp);

			}


	if (tmStatus.CzasUplynal())
		status = "";

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(14);
	text.setPosition(sf::Vector2f(5, 505));


	if (stanGry == W_TRAKCIE)
	{
		std::string naPowierzchni = std::string("(") + std::to_string(plansza->NaPowierzchni) + "/20) ";
		text.setString(naPowierzchni + (std::string(tryb == GRACZ ? "RUCHY KOMPUTERA  " : "RUCHY GRACZA  ") + status).c_str());
		if (czyjRuch == tryb)
			text.setFillColor(sf::Color::Red);
		else
			text.setFillColor(sf::Color::Black);
	}
	else
	{
		text.setString(stanGry == WYGRAL_KOMPUTER ? "Wygral komputer" : "Wygral czlowiek");
		if (stanGry == WYGRAL_KOMPUTER && tryb == KOMPUTER || stanGry == WYGRAL_GRACZ && tryb == GRACZ)
			text.setFillColor(sf::Color(0, 192, 0));
		else
			text.setFillColor(sf::Color::Red);
	}

	draw(text);


	if (stanGry == W_TRAKCIE && tryb == GRACZ && czyjRuch == KOMPUTER && tmRuchKomputera.CzasUplynal())
	{
	odNowa:

		if (ruchyKomputera.size() == 0)
		{

			int rx, ry;
			do
			{
				rx = 1.0 * std::rand() / RAND_MAX * Plansza::ROZMIAR;
				ry = 1.0 * std::rand() / RAND_MAX * Plansza::ROZMIAR;


			} while (pamiec[rx][ry]);


			pamiec[rx][ry] = true;


			if (plansza->CzyZajety(rx, ry))
			{


				plansza->Trafiony(rx, ry);
				Statek *s = plansza->komorki[rx][ry].statek;

				if (s->zatopiony)
				{
					int wys = s->kierunek == Statek::PIONOWY ? s->dlugosc : 1;
					int szer = s->kierunek == Statek::POZIOMY ? s->dlugosc : 1;
					int x1, y1, x2, y2;
					if (rx == 0) x1 = 0; else x1 = rx - 1;
					if (rx + szer > 9) x2 = 9; else x2 = rx + szer;
					if (ry == 0) y1 = 0; else y1 = ry - 1;
					if (ry + wys == 9) y2 = 9; else y2 = ry + wys;

					for (int i = x1; i <= x2; i++)
						for (int j = y1; j <= y2; j++)
							pamiec[i][j] = true;
					if (plansza->NaPowierzchni == 0)
					{
						stanGry = WYGRAL_KOMPUTER;
						oponent->stanGry = WYGRAL_KOMPUTER;
					}
				}


				ruchyKomputera.push_front({ rx,ry });
				tmRuchKomputera.Ustaw(1);

			}
			else {
				plansza->komorki[rx][ry].odkryta = true;
				czyjRuch = GRACZ;
				ustawStatus("Komputer spudlowal!");
			}
		}

		else if (ruchyKomputera.size() >= 1)
		{

			Polozenie ostatniRuch = ruchyKomputera.front();
			bool ok = false;
			int proby = 4;

			int rx = ostatniRuch.first;
			int ry = ostatniRuch.second;

			if (ruchyKomputera.size() == 1)
			{
				do
				{
					kierunek = 1.0 * std::rand() / RAND_MAX * 4;

					ok = true;


					if (kierunek == 0 && ry == 0)
						ok = false;
					else if (kierunek == 1 && rx == 9)
						ok = false;
					else if (kierunek == 2 && ry == 9)
						ok = false;
					else if (kierunek == 3 && rx == 0)
						ok = false;
					proby--;
				} while (!ok && proby > 0);

				if (!ok)
				{

					ruchyKomputera.clear();
					goto odNowa;
				}

			}


			switch (kierunek)
			{
			case 0: ry--; break;
			case 1: rx++; break;
			case 2: ry++; break;
			case 3: rx--; break;
			}

			if (pamiec[rx][ry] || rx < 0 || rx > 9 || ry < 0 || ry > 9)
			{
				ruchyKomputera.clear();
				goto odNowa;
			}

			else if (plansza->komorki[rx][ry].statek && plansza->komorki[rx][ry].statek->zatopiony)
			{
				ruchyKomputera.clear();
				goto odNowa;
			}

			pamiec[rx][ry] = true;

			if (plansza->CzyZajety(rx, ry))
			{
				plansza->Trafiony(rx, ry);

				Statek *s = plansza->komorki[rx][ry].statek;
				if (s->zatopiony)
				{
					int wys = s->kierunek == Statek::PIONOWY ? s->dlugosc : 1;
					int szer = s->kierunek == Statek::POZIOMY ? s->dlugosc : 1;
					int x1, y1, x2, y2;
					if (rx == 0) x1 = 0; else x1 = rx - 1;
					if (rx + szer > 9) x2 = 9; else x2 = rx + szer;
					if (ry == 0) y1 = 0; else y1 = ry - 1;
					if (ry + wys == 9) y2 = 9; else y2 = ry + wys;

					for (int i = x1; i <= x2; i++)
						for (int j = y1; j <= y2; j++)
							pamiec[i][j] = true;
					if (plansza->NaPowierzchni == 0)
					{
						stanGry = WYGRAL_KOMPUTER;
						oponent->stanGry = WYGRAL_KOMPUTER;
					}
				}

				ruchyKomputera.push_front({ rx,ry });
				ustawStatus(std::string("Trafiony ") + NazwaKomorki(rx, ry));
				tmRuchKomputera.Ustaw(1);
			}
			else
			{
				czyjRuch = GRACZ;
				ustawStatus("Komputer spudlowal!");
				plansza->komorki[rx][ry].odkryta = true;
				ruchyKomputera.clear();
			}
		}
	}

	display();
}

