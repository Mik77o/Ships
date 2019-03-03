#include "Statek.h"
#include <cstdlib>

Statek::Statek(int dlugosc, Kierunek kierunek, int x, int y)
	: dlugosc(dlugosc), kierunek(kierunek), x(x), y(y)
{

	kolor.r = 1.0 * std::rand() / RAND_MAX * 194 + 32;
	kolor.g = 1.0 * std::rand() / RAND_MAX * 194 + 32;
	kolor.b = 1.0 * std::rand() / RAND_MAX * 194 + 32;
	zatopiony = false;
}

Statek::~Statek()
{

}
