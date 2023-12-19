#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Figures.h"
#include "RobertsAlgorithm.h"
#include "Shadow.h"

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPosition = { 0, 0 };
	Shadow shadow;

	while (true)
	{
		shadow.paint();
		std::cout << "Controls:\n"
			<< "Move:\n"
			<< "\tX: a, d\n"
			<< "\tY: w, s\n"
			<< "\tZ: z, x\n"
			<< "Scale:\n"
			<< "\t+, -\n"
			<< "Rotate:\n"
			<< "\tX: 2, 8\n"
			<< "\tY: 4, 6\n"
			<< "\tZ: q, e\n"
			<< "Exit:"
			<< "\t0\n";

		SetConsoleCursorPosition(hConsole, cursorPosition);
		switch (_getch())
		{
		case 'w': case 'W':
			shadow.move(Y, MINUS);
			break;
		case 'a': case 'A':
			shadow.move(X, MINUS);
			break;
		case 's': case 'S':
			shadow.move(Y, PLUS);
			break;
		case 'd': case 'D':
			shadow.move(X, PLUS);
			break;
		case 'x': case 'X':
			shadow.move(Z, PLUS);
			break;
		case 'z': case 'Z':
			shadow.move(Z, MINUS);
			break;
		case '=': case '+':
			shadow.scale(PLUS);
			break;
		case '-': case '_':
			shadow.scale(MINUS);
			break;
		case '2':
			shadow.rotate(X, PLUS);
			break;
		case '8':
			shadow.rotate(X, MINUS);
			break;
		case '4':
			shadow.rotate(Y, PLUS);
			break;
		case '6':
			shadow.rotate(Y, MINUS);
			break;
		case 'q': case 'Q':
			shadow.rotate(Z, PLUS);
			break;
		case 'e': case 'E':
			shadow.rotate(Z, MINUS);
			break;
		case '0':
			return 0;
		default:
			continue;
		}
	}
}