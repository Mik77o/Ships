#pragma once
#include <ctime>


class Timer
{
public:
	Timer();


	void Ustaw(int czas);

	void Reset();


	bool CzasUplynal()
	{
		if (aktywny && std::time(0) > start + czas)
		{

			if (!odczyt)
			{
				odczyt = true;
				return true;
			}
			return false;
		}
		return false;
	}

private:
	bool odczyt;
	bool aktywny;
	std::time_t start;
	std::time_t czas;
};
