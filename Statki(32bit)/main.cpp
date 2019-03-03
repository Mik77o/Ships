#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Plansza.h"
#include "StatkiWindow.h"
#include "Gra.h"

int main()
{
	std::srand(std::time(0));
	Plansza planszaGracza, planszaKomputera;
	StatkiWindow computerWindow(&planszaKomputera, StatkiWindow::KOMPUTER);
	StatkiWindow playerWindow(&planszaGracza, StatkiWindow::GRACZ);
	StatkiWindow::Tryb ruch = StatkiWindow::GRACZ;

	computerWindow.UstawOponenta(&playerWindow);
	playerWindow.UstawOponenta(&computerWindow);


	sf::VideoMode dm = sf::VideoMode::getDesktopMode();
	const int rozmiar_okna = 500;
	playerWindow.setPosition(sf::Vector2i(dm.width / 2 - rozmiar_okna - 20, dm.height / 2 - rozmiar_okna / 2));
	computerWindow.setPosition(sf::Vector2i(dm.width / 2 + 20, dm.height / 2 - rozmiar_okna / 2));

	while (playerWindow.isOpen() && computerWindow.isOpen()) {

		playerWindow.tick(ruch);
		computerWindow.tick(ruch);


	}
}
