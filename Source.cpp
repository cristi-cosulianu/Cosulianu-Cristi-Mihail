#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <conio.h>
#include <limits>
#include <fstream>
using namespace std;

ifstream f("date.in");

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ENTER 13
#define SPACE 32
#define BACKSPACE 8
#define ESC 27


struct matrix
{
	char valoare;
	bool demascat;
	bool steag;
};

struct map
{
	int linii;
	int coloane;
	matrix matrice[180][180] = { 0 };
};
map mapa;


HANDLE point;

int nrMine;
int nrSteaguri = 0;

bool gameOver = 0;
bool retry = 0;
bool win = 0;

void afisare();
void initializareMapa();
void miscare();
void generareHarta();
void dificultateCustom();
void alegeDificultate();

BOOL seteazaDimensiuneaFontului(COORD dwFontSize) 
{
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info{ sizeof(CONSOLE_FONT_INFOEX) };
	if (!GetCurrentConsoleFontEx(output, false, &info))
		return false;
	info.dwFontSize = dwFontSize;
	return SetCurrentConsoleFontEx(output, false, &info);
}

void seteazaDimensiuneaFerestrei(int inaltime, int latime)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); 

	MoveWindow(console, r.left, r.top, inaltime, latime, TRUE);
}

void fullScreen()
{
	::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
}

void ascundeCursorul()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void coloreaza(int cod_culoare)
{
	point = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(point, cod_culoare);
}

void setCursor(int x, int y)
{
	COORD pozitie;
	pozitie.X = x;
	pozitie.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pozitie);
}

void initializareMapa()
{
	for (int i = 0;i <= 50;i++)
	{
		for (int j = 0;j <= 50;j++)
		{
			mapa.matrice[i][j].valoare = 0;
			mapa.matrice[i][j].demascat = false;
			mapa.matrice[i][j].steag = false;
		}
	}

	mapa.matrice[0][0].valoare = '+';
	mapa.matrice[0][0].demascat = true;
	mapa.matrice[mapa.linii * 2 + 2][0].valoare = '+';
	mapa.matrice[mapa.linii * 2 + 2][0].demascat = true;
	mapa.matrice[mapa.linii * 2 + 2][mapa.coloane * 2 + 2].valoare = '+';
	mapa.matrice[mapa.linii * 2 + 2][mapa.coloane * 2 + 2].demascat = true;
	mapa.matrice[0][mapa.coloane * 2 + 2].valoare = '+';
	mapa.matrice[0][mapa.coloane * 2 + 2].demascat = true;

	for (int i = 1;i <= 2 * mapa.linii + 1;i++)
	{
		mapa.matrice[i][0].valoare = '|';
		mapa.matrice[i][0].demascat = true;
		mapa.matrice[i][2 * mapa.coloane + 2].valoare = '|';
		mapa.matrice[i][2 * mapa.coloane + 2].demascat = true;
	}
	for (int i = 1;i <= 2 * mapa.coloane + 1;i++)
	{
		mapa.matrice[0][i].valoare = '-';
		mapa.matrice[0][i].demascat = true;
		mapa.matrice[2 * mapa.linii + 2][i].valoare = '-';
		mapa.matrice[2 * mapa.linii + 2][i].demascat = true;
	}
	for (int i = 1;i <= mapa.linii * 2 + 1;i++)
	{
		for (int j = 1;j <= mapa.coloane * 2 + 1;j++)
		{
			if (i % 2 == 1 || j % 2 == 1)
				mapa.matrice[i][j].demascat = true;
			else
			{
				mapa.matrice[i][j].demascat = false;
				mapa.matrice[i][j].valoare = '0';
			}

		}
	}
}

void miscare()
{
	int x = 1, y = 1;
	mapa.matrice[x * 2][y * 2 - 1].valoare = '<';
	mapa.matrice[x * 2][y * 2 + 1].valoare = '>';

	while (gameOver == 0 && win == 0)
	{
		int c = 0;
	scan:
		c = 0;
		switch (c = _getch())
		{
		case KEY_LEFT:
		{
			if (y - 1== 0)
				goto scan;
			else
			{
				swap(mapa.matrice[x * 2][y * 2 - 1], mapa.matrice[x * 2][(y - 1) * 2 - 1]);
				swap(mapa.matrice[x * 2][y * 2 + 1], mapa.matrice[x * 2][(y - 1) * 2 + 1]);

				coloreaza(117);

				setCursor(y * 2 - 1 + 6, x * 2 + 6);
				printf("<");
				setCursor(y * 2 - 1 + 8, x * 2 + 6);
				printf(">");
				setCursor(y * 2 - 1 + 10, x * 2 + 6);
				printf(" ");

				y--;
			}
			break;
		}
		case KEY_RIGHT:
		{
			if (y + 1 > mapa.coloane)
				goto scan;
			else
			{
				swap(mapa.matrice[x * 2][y * 2 + 1], mapa.matrice[x * 2][(y + 1) * 2 + 1]);
				swap(mapa.matrice[x * 2][y * 2 - 1], mapa.matrice[x * 2][(y + 1) * 2 - 1]);

				coloreaza(117);

				setCursor(y * 2 - 1 + 8, x * 2 + 6);
				printf(" ");
				setCursor(y * 2 - 1 + 10, x * 2 + 6);
				printf("<");
				setCursor(y * 2 - 1 + 12, x * 2 + 6);
				printf(">");

				y++;
			}
			break;
		}
		case KEY_DOWN:
		{
			if (x + 1 > mapa.linii)
				goto scan;
			else
			{
				swap(mapa.matrice[x * 2][y * 2 - 1], mapa.matrice[(x + 1) * 2][y * 2 - 1]);
				swap(mapa.matrice[x * 2][y * 2 + 1], mapa.matrice[(x + 1) * 2][y * 2 + 1]);

				coloreaza(117);

				setCursor(y * 2 - 1 + 8, x * 2 + 6);
				printf(" ");
				setCursor(y * 2 - 1 + 10, x * 2 + 6);
				printf(" ");
				setCursor(y * 2 - 1 + 8, x * 2 + 8);
				printf("<");
				setCursor(y * 2 - 1 + 10, x * 2 + 8);
				printf(">");

				x++;
			}
			break;
		}
		case KEY_UP:
		{
			if (x - 1 == 0)
				goto scan;
			else
			{

				swap(mapa.matrice[x * 2][y * 2 - 1], mapa.matrice[(x - 1) * 2][y * 2 - 1]);
				swap(mapa.matrice[x * 2][y * 2 + 1], mapa.matrice[(x - 1) * 2][y * 2 + 1]);

				coloreaza(117);

				setCursor(y * 2 - 1 + 8, x * 2 + 6);
				printf(" ");
				setCursor(y * 2 - 1 + 10, x * 2 + 6);
				printf(" ");
				setCursor(y * 2 - 1 + 8, x * 2 + 4);
				printf("<");
				setCursor(y * 2 - 1 + 10, x * 2 + 4);
				printf(">");
				
				x--;
			}
			break;
		}
		case ENTER:
		{

			if (mapa.matrice[x * 2][y * 2].valoare == 'M')
			{
				system("cls");

				coloreaza(127);

				COORD marimeFont;
				marimeFont.X = 40;
				marimeFont.Y = 40;
				seteazaDimensiuneaFontului(marimeFont);

				printf("\n\t GAME OVER - You just hit a mine.");

				printf("\n\t If you want to retry press Enter.");

				c = _getch();
				if (c == ENTER)
					retry = 1;
				gameOver = 1;
			}

			mapa.matrice[x * 2][y * 2].demascat = true;

			setCursor(y * 2 + 8, x * 2 + 6);
			switch (mapa.matrice[x*2][y*2].valoare)
			{
			case '0': {coloreaza(127); break;}  //Alb
			case '1': {coloreaza(121); break;}  //Albastru
			case '2': {coloreaza(114); break;}  //Verde
			case '3': {coloreaza(124); break;}  //Rosu
			case '4': {coloreaza(123); break;}  //Albastru Deschis
			case '5': {coloreaza(116); break;}  //Visiniu
			case '6': {coloreaza(113); break;}  //Albastru Ichis
			case '7': {coloreaza(122); break;}  //Verde Deschis
			case '8': {coloreaza(125); break;}  //Roz
			}
			cout<<mapa.matrice[x*2][y*2].valoare;

			break;
		}
		case SPACE:
		{
			if (mapa.matrice[x * 2][y * 2].demascat == false)
			{
				nrSteaguri++;
				mapa.matrice[x * 2][y * 2].steag = 1;
				setCursor(y * 2 + 8, x * 2 + 6);
				coloreaza(117);
				printf("S");
			}
			break;
		}
		case BACKSPACE:
		{
			if (mapa.matrice[x * 2][y * 2].steag == 1)
			{
				mapa.matrice[x * 2][y * 2].steag = 0;
				nrSteaguri--;
				setCursor(y * 2 + 8, x * 2 + 6);
				coloreaza(127);
				printf("X");
			}
			break;
		}
		case ESC:
		{
			printf("\n");
			setCursor(6 , mapa.linii * 2 + 10);
			coloreaza(139);
			exit(0);
			break;
		}
		}
		ascundeCursorul();
	}
}

void numeroteazaVecinii(int x, int y)
{
	if (y - 1 > 0 && x - 1 > 0)
	{
		if (strchr("M+_|-", mapa.matrice[(x - 1) * 2][(y - 1) * 2].valoare) == 0)
			mapa.matrice[(x - 1) * 2][(y - 1) * 2].valoare++;
	}
	if (x - 1 > 0)
	{
		if (strchr("M+_|-", mapa.matrice[(x - 1) * 2][y * 2].valoare) == 0)
			mapa.matrice[(x - 1) * 2][y * 2].valoare++;
	}
	if (y <= mapa.coloane&& x - 1>0)
	{
		if (strchr("M+_|-", mapa.matrice[(x - 1) * 2][(y + 1) * 2].valoare) == 0)
			mapa.matrice[(x - 1) * 2][(y + 1) * 2].valoare++;
	}
	if (y <= mapa.coloane)
	{
		if (strchr("M+_|-", mapa.matrice[x * 2][(y + 1) * 2].valoare) == 0)
			mapa.matrice[x * 2][(y + 1) * 2].valoare++;
	}
	if (x <= mapa.linii && y <= mapa.coloane)
	{
		if (strchr("M+_|-", mapa.matrice[(x + 1) * 2][(y + 1) * 2].valoare) == 0)
			mapa.matrice[(x + 1) * 2][(y + 1) * 2].valoare++;
	}
	if (x <= mapa.linii)
	{
		if (strchr("M+_|-", mapa.matrice[(x + 1) * 2][y * 2].valoare) == 0)
			mapa.matrice[(x + 1) * 2][y * 2].valoare++;
	}
	if (x <= mapa.linii&&y - 1>0)
	{
		if (strchr("M+_|-", mapa.matrice[(x + 1) * 2][(y - 1) * 2].valoare) == 0)
			mapa.matrice[(x + 1) * 2][(y - 1) * 2].valoare++;
	}
	if (y - 1>0)
	{
		if (strchr("M+_|-", mapa.matrice[x * 2][(y - 1) * 2].valoare) == 0)
			mapa.matrice[x * 2][(y - 1) * 2].valoare++;
	}
}

void generareHarta()
{
	int x, y, i = 0;

	if (mapa.linii + mapa.coloane <= 20)
		nrMine = (mapa.linii + mapa.coloane);
	else if (mapa.linii + mapa.coloane <= 40)
		nrMine = (mapa.linii + mapa.coloane) + 10;
	else
		nrMine = (mapa.linii + mapa.coloane) * 2;

	srand(time(nullptr));
	while (i < nrMine)
	{
		x = rand() % (mapa.linii * 2) + 1;
		y = rand() % (mapa.coloane * 2) + 1;
		if (mapa.matrice[x * 2][y * 2].valoare != 'M'&&x > 1 && y > 1 && x <= mapa.linii&&y <= mapa.coloane)
		{
			mapa.matrice[x * 2][y * 2].valoare = 'M';
			i++;
		}
	}
	for (i = 1;i <= mapa.linii;i++)
	{
		for (int j = 1;j <= mapa.coloane;j++)
		{
			if (mapa.matrice[i * 2][j * 2].valoare == 'M')
			{
				numeroteazaVecinii(i, j);
			}
		}
	}
}

void afisare()
{
	system("cls");

	/*COORD marimeFont;
	marimeFont.X = 20;
	marimeFont.Y = 20;
	seteazaDimensiuneaFontului(marimeFont);*/

	printf("\n\t Utilizati tastele Up,Down,Left and Right pentru a misca cursorul.");
	printf("\n\t Utilizati tast Enter pentru a descoperi casutele, Space pentru a plasa");
	printf("\n\t steaguri in locurile in care presupuneti ca sunt Mine si BackSpace pentru");
	printf("\n\t a sterge steagurile si pentru a reveni la forma initiala.\n\n");

	int x = 1, y = 1;

	coloreaza(117);
	mapa.matrice[x * 2][y * 2 - 1].valoare = '<';
	mapa.matrice[x * 2][y * 2 + 1].valoare = '>';     

	coloreaza(127);
	for (int i = 0;i <= mapa.linii * 2 + 2;i++)
	{
		printf("\t");
		for (int j = 0;j <= mapa.coloane * 2 + 2;j++)
		{
			if (mapa.matrice[i][j].demascat == true || strchr("+_|<>-", mapa.matrice[i][j].valoare) != 0)
			{
				if (strchr("<>", mapa.matrice[i][j].valoare) != 0)
				{
					coloreaza(117);
					printf("%c", mapa.matrice[i][j].valoare);
				}
				else
				{
					coloreaza(127);
					printf("%c", mapa.matrice[i][j].valoare);
				}
			}
			else if (mapa.matrice[i][j].steag == 1)
			{
				coloreaza(117);                                                                                    // Coloreaza cu Mov;
				printf("S");
			}
			else
			{
				coloreaza(127);                                                                                    // Coloreaza cu Alb;
				printf("X");
			}
		}
		printf("\n");
	}
}

void dificultateCustom()
{
	int linii, coloane;
	system("cls");
	printf("\n\t Introduceti doua valori, cu spatiu intre ele, \n");
	printf("\taflate intre 9 si 50 pentru a seta dimensiunile \n\n\t");
	scanf_s("%d", &linii);
	scanf_s("%d", &coloane);
	if (linii < 9 || linii>50 || coloane < 9 || coloane>50)
	{
		system("cls");
		printf("\n\t Ati introdus valori incorecte. Sa mai incercam odata!\n");
		dificultateCustom();
	}
	else
	{
		mapa.linii = linii;
		mapa.coloane = coloane;
	}
}

void alegeDificultate()
{


	COORD marimeFont;
	marimeFont.X = 20;
	marimeFont.Y = 20;
	seteazaDimensiuneaFontului(marimeFont);
	/*seteazaDimensiuneaFerestrei(2000, 1200);*/

	int alegere;
	system("Color 8F");
	printf("\n\t 1. Easy 9x9");
	printf("\n\t 2. Mediu 16x16");
	printf("\n\t 3. Hard 40x40");
	printf("\n\t 4. Custom \n");

	scanf_s("%d", &alegere);
	switch (alegere)
	{
	case 1: {mapa.linii = 9;mapa.coloane = 9;break;}
	case 2: {mapa.linii = 16;mapa.coloane = 16;break;}
	case 3: {mapa.linii = 40;mapa.coloane = 40;break;}
	case 4: {dificultateCustom();break;}
	default:
	{
		system("cls");
		printf("\v\tNu ati ales corect dificultatea! Incercati din nou!\n");
		alegeDificultate();
	}
	}
}

int main()
{

	do
	{
		system("cls");
		gameOver = 0;
		retry = 0;
		win = 0;
		fullScreen();
		alegeDificultate();
		initializareMapa();
		generareHarta();
		afisare();
		miscare();

	} while (retry == 1 && (gameOver == 1 || win == 1));

	/*for (int i = 0;i <= 1000;i++)
	{
		coloreaza(i);
		cout << i<<" aaaaaaaaaaaa"<<"\n";
	}*/
	return 0;
}