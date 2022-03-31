#include "LostArkHoningCalculator/Utils.h"
#include <Windows.h>


void* hConsole = nullptr;

void setConsoleColour(int colour)
{
	if (hConsole == nullptr)
	{
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	SetConsoleTextAttribute(hConsole, colour);
}

float howMuchCheaper(float cheaperTarget, float expensiveSource)
{
	float cheaper = (1.0f - cheaperTarget / expensiveSource) * 100.0f;
	if (isMore(cheaper, 0.0f))
		setConsoleColour(10);
	else
		setConsoleColour(12);
	return cheaper;
}
