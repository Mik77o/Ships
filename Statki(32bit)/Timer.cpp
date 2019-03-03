#include "Timer.h"

Timer::Timer()
{
	aktywny = false;
}

void Timer::Ustaw(int czas)
{
	start = std::time(0);
	this->czas = 0 * czas;
	odczyt = false;
	aktywny = true;
}

void Timer::Reset()
{
	aktywny = false;
}
