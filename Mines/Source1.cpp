#include <time.h>
#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <limits>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ENTER 13
#define SPACE 32
#define BACKSPACE 8
#define ESC 27
#define UNU 49
#define	DOI 50
#define TREI 51 
#define	PATRU 52

struct matrix
{
	char valoare=0;
	bool demascat=0;
	bool steag=0;
	int formatiuneMina = 0;
};

struct map
{
	int linii;
	int coloane;
	matrix matrice[200][200] = { 0 };
};
map mapa;

struct punct
{
	int x, y;
	bool viz;
};


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
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);
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

bool existaInLista(punct lista[], int sf, int x, int y)
{
	for (int i = 0;i < sf;i++)
	{
		if (x == lista[i].x&&y == lista[i].y)
			return 1;
	}
	return 0;
}

void afisarePunct(int x, int y)
{
	mapa.matrice[x * 2][y * 2].demascat = true;
	setCursor(y * 2 + 8, x * 2 + 6);
	cout << mapa.matrice[x * 2][y * 2].valoare;
}

void adaugaTotiVecinii(punct lista[], int inc, int &sf)
{

	if (lista[inc].x > 1 && lista[inc].y > 1)
	{
		if (existaInLista(lista, sf, lista[inc].x - 1, lista[inc].y - 1) == 0)
		{
			sf++;
			lista[sf].x = lista[inc].x - 1;
			lista[sf].y = lista[inc].y - 1;
		}
	}
	if (lista[inc].x > 1)
	{
		if (existaInLista(lista, sf, lista[inc].x - 1, lista[inc].y) == 0)
		{
			sf++;
			lista[sf].x = lista[inc].x - 1;
			lista[sf].y = lista[inc].y;
		}
	}
	if (lista[inc].x > 1 && lista[inc].y < mapa.coloane)
	{
		if (existaInLista(lista, sf, lista[inc].x - 1, lista[inc].y + 1) == 0)
		{
			sf++;
			lista[sf].x = lista[inc].x - 1;
			lista[sf].y = lista[inc].y + 1;
		}
	}
	if (lista[inc].y < mapa.coloane)
	{
		if (existaInLista(lista, sf, lista[inc].x, lista[inc].y + 1) == 0)
		{
			sf++;
			lista[sf].x = lista[inc].x;
			lista[sf].y = lista[inc].y + 1;
		}
	}
	if (lista[inc].y < mapa.coloane && lista[inc].x < mapa.linii)
	{
		if (existaInLista(lista, sf, lista[inc].x + 1, lista[inc].y + 1) == 0)
		{
			sf++;
			lista[sf].x = lista[inc].x + 1;
			lista[sf].y = lista[inc].y + 1;
		}
	}
	if (lista[inc].x < mapa.linii)
	{
		if (existaInLista(lista, sf, lista[inc].x + 1, lista[inc].y) == 0)
		{
			sf++;
			lista[sf].x = lista[inc].x + 1;
			lista[sf].y = lista[inc].y;
		}
	}
	if (lista[inc].x < mapa.linii&&lista[inc].y>1)
	{
		if (existaInLista(lista, sf, lista[inc].x + 1, lista[inc].y - 1) == 0)
		{
			sf++;
			lista[sf].x = lista[inc].x + 1;
			lista[sf].y = lista[inc].y - 1;
		}
	}
	if (lista[inc].y > 1)
	{
		if (existaInLista(lista, sf, lista[inc].x, lista[inc].y - 1) == 0)
		{
			sf++;
			lista[sf].x = lista[inc].x;
			lista[sf].y = lista[inc].y - 1;
		}
	}
}

void descopera(int x, int y)
{
	int inc, sf;
	punct lista[121*121];
	lista[0].x = x;
	lista[0].y = y;

	inc = sf = 0;
	while (inc <= sf)
	{
		switch (mapa.matrice[lista[inc].x * 2][lista[inc].y * 2].valoare)
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
		afisarePunct(lista[inc].x, lista[inc].y);
		if (mapa.matrice[lista[inc].x * 2][lista[inc].y * 2].valoare == '0')
		{
			adaugaTotiVecinii(lista, inc, sf);
		}
		inc++;
	}
}

bool verificaSteaguri()
{
	for (int i = 0;i <= mapa.linii * 2;i++)
	{
		for (int j = 0;j <= mapa.coloane * 2;j++)
		{
			if (mapa.matrice[i][j].steag == 1 && mapa.matrice[i][j].valoare != 'M')
				return false;
		}
	}
	return true;
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
			if (y - 1 == 0)
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

			if (mapa.matrice[x * 2][y * 2].valoare == 'M'&&mapa.matrice[x*2][y*2].steag==0)
			{
				mapa.matrice[x * 2][y * 2 - 1].valoare = ' ';
				mapa.matrice[x * 2][y * 2 + 1].valoare = ' ';

				system("cls");

				COORD marimeFont;
				marimeFont.X = 40;
				marimeFont.Y = 40;
				seteazaDimensiuneaFontului(marimeFont);

				system("Color 8F");
				coloreaza(139);

				printf("\n\t GAME OVER - Ai lovit o mina. :( ");

				printf("\n\t Daca vrei sa incerci dinnou apasa Enter.");

				c = _getch();
				if (c == ENTER)
					retry = 1;
				gameOver = 1;
			}

			mapa.matrice[x * 2][y * 2].demascat = true;

			switch (mapa.matrice[x * 2][y * 2].valoare)
			{
			case '0': {descopera(x, y); coloreaza(127); break;}  //Alb
			case '1': {coloreaza(121); break;}  //Albastru
			case '2': {coloreaza(114); break;}  //Verde
			case '3': {coloreaza(124); break;}  //Rosu
			case '4': {coloreaza(123); break;}  //Albastru Deschis
			case '5': {coloreaza(116); break;}  //Visiniu
			case '6': {coloreaza(113); break;}  //Albastru Ichis
			case '7': {coloreaza(122); break;}  //Verde Deschis
			case '8': {coloreaza(125); break;}  //Roz
			}
			setCursor(y * 2 + 8, x * 2 + 6);
			cout << mapa.matrice[x * 2][y * 2].valoare;

			break;
		}
		case SPACE:
		{
			if (mapa.matrice[x * 2][y * 2].demascat == false && mapa.matrice[x * 2][y * 2].steag == 0 && nrSteaguri<nrMine)
			{
				mapa.matrice[x * 2][y * 2].steag = 1;
				setCursor(y * 2 + 8, x * 2 + 6);
				coloreaza(117);
				printf("S");

				coloreaza(139);
				nrSteaguri++;
				setCursor(6, mapa.linii * 2 + 10);
				printf("\t Numar de steaguri plasate: %d  ", nrSteaguri);
				printf("\n\t Numar de de mine ramase: %d  ", nrMine - nrSteaguri);

			}
			if (nrSteaguri == nrMine)
			{
				if (verificaSteaguri())
				{
					system("cls");

					COORD marimeFont;
					marimeFont.X = 40;
					marimeFont.Y = 40;
					seteazaDimensiuneaFontului(marimeFont);

					coloreaza(139);

					printf("\n\t Bravo!!! Ai castigat!!!");
					printf("\n\t Daca vrei sa joci dinnou apasa ");

					coloreaza(142);

					printf("Enter.");

					c = _getch();
					if (c == ENTER)
						retry = 1;
					win = 1;
				}
				else
				{
					coloreaza(142);
					setCursor(6, mapa.linii * 2 + 12);
					printf("\n\t Steagurile au fost plasate incorect! Incearca sa corectezi.");
				}
			}
			break;
		}
		case BACKSPACE:
		{
			if (nrSteaguri == nrMine)
			{
				coloreaza(142);
				setCursor(6, mapa.linii * 2 + 12);
				printf("\n\t                                                              ");
			}
			if (mapa.matrice[x * 2][y * 2].steag == 1)
			{
				mapa.matrice[x * 2][y * 2].steag = 0;
				nrSteaguri--;
				setCursor(y * 2 + 8, x * 2 + 6);
				coloreaza(112);
				printf("X");

				coloreaza(139);
				setCursor(6, mapa.linii * 2 + 10);
				printf("\t Numar de steaguri plasate: %d  ", nrSteaguri);
				printf("\n\t Numar de de mine ramase: %d  ", nrMine - nrSteaguri);
			}
			break;
		}
		case ESC:
		{
			setCursor(6, mapa.linii * 2 + 13);
			printf("\t ");
			coloreaza(142);
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

bool vecinMina(int x, int y)
{
	int veciniMina = 0;
	if (y - 1 > 0 && x - 1 > 0)
	{
		if (mapa.matrice[(x - 1) * 2][(y - 1) * 2].valoare == 'M')
			veciniMina++;
	}
	if (x - 1 > 0)
	{
		if (mapa.matrice[(x - 1) * 2][y * 2].valoare == 'M')
			veciniMina++;
	}
	if (y <= mapa.coloane&& x - 1>0)
	{
		if (mapa.matrice[(x - 1) * 2][(y + 1) * 2].valoare == 'M')
			veciniMina++;
	}
	if (y <= mapa.coloane)
	{
		if (mapa.matrice[x * 2][(y + 1) * 2].valoare == 'M')
			veciniMina++;
	}
	if (x <= mapa.linii && y <= mapa.coloane)
	{
		if (mapa.matrice[(x + 1) * 2][(y + 1) * 2].valoare == 'M')
			veciniMina++;
	}
	if (x <= mapa.linii)
	{
		if (mapa.matrice[(x + 1) * 2][y * 2].valoare == 'M')
			veciniMina++;
	}
	if (x <= mapa.linii&&y - 1>0)
	{
		if (mapa.matrice[(x + 1) * 2][(y - 1) * 2].valoare == 'M')
			veciniMina++;
	}
	if (y - 1>0)
	{
		if (mapa.matrice[x * 2][(y - 1) * 2].valoare == 'M')
		{
			veciniMina++;
		}
	}
	if (veciniMina > 3)
		return 0;
	else
		return 1;
}

bool densitateMine()
{
	return 1;
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
			if (vecinMina(x, y) && densitateMine())
			{
				mapa.matrice[x * 2][y * 2].valoare = 'M';
				i++;
			}
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
	COORD marimeFont;
	marimeFont.X = 20;
	marimeFont.Y = 30;
	seteazaDimensiuneaFontului(marimeFont);

	
	cout << "  _____ _         _____                            " << '\n';
	cout << " |     |_|___ ___|   __|_ _ _ ___ ___ ___ ___ ___  " << '\n';
	cout << " | | | | |   | -_|__   | | | | -_| -_| . | -_|  _| " << '\n';
	cout << " |_|_|_|_|_|_|___|_____|_____|___|___|  _|___|_|   " << '\n';
	cout << "                                     |_|        " << '\n';

	cout << char(200);

	coloreaza(139);
	printf("\n\t Utilizati tastele ");
	coloreaza(142);
	cout << "Up";
	coloreaza(139);
	cout << ',';
	coloreaza(142);
	cout << "Down";
	coloreaza(139);
	cout << ',';
	coloreaza(142);
	cout << "Left";
	coloreaza(139);
	cout << " and ";
	coloreaza(142);
	cout << "Right";
	coloreaza(139);
	printf(" pentru a misca cursorul. \n\t Utilizati tasta ");
	coloreaza(142);
	cout << "Enter ";
	coloreaza(139);
	printf("pentru a descoperi casutele,");
	coloreaza(142);
	cout << " Space ";
	coloreaza(139);
	printf("pentru a plasa\n\t steaguri in locurile in care presupuneti ca sunt mine si");
	coloreaza(142);
	cout << " BackSpace ";
	coloreaza(139);
	printf("pentru\n\t a sterge steagurile si pentru a reveni la forma initiala.\n\n");

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
				coloreaza(112);                                                                                    // Coloreaza cu Alb;
				printf("X");
			}
		}
		printf("\n");
	}

	coloreaza(139);
	setCursor(6, mapa.linii * 2 + 10);
	printf("\t Numar de steaguri plasate: %d  ", nrSteaguri);
	printf("\n\t Numar de de mine ramase: %d  ", nrMine - nrSteaguri);
}

void dificultateCustom()
{
	coloreaza(139);
	int linii, coloane;
	system("cls");
	printf("\n\t Introduceti doua valori, cu spatiu intre ele, \n");
	printf("\t aflate intre 9 si 60 pentru a seta dimensiunile \n\n\t");
	scanf_s("%d", &linii);
	scanf_s("%d", &coloane);
	if (linii < 9 || linii>60 || coloane < 9 || coloane>60)
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
	marimeFont.X = 40;
	marimeFont.Y = 40;
	seteazaDimensiuneaFontului(marimeFont);

	int alegere;
	system("Color 8F");
	coloreaza(139);
	printf("\n\t Apasati ");
	coloreaza(142);
	cout << "1";
	coloreaza(139);
	cout << ",";
	coloreaza(142);
	cout << "2";
	coloreaza(139);
	cout << ",";
	coloreaza(142);
	cout << "3";
	coloreaza(139);
	cout << " sau ";
	coloreaza(142);
	cout << "4";
	coloreaza(139);
	printf(" pentru a va alege dimensiunea tablei de joc.\n");
	printf("\n\t 1. Usor 9x9.");
	printf("\n\t 2. Mediu 16x16.");
	printf("\n\t 3. Greu 40x40.");
	printf("\n\t 4. Alegeti dimensiunile. \n");
	printf("\n\n\t Apasati tasta ");
	coloreaza(142);
	cout << "Esc";
	coloreaza(139);
	printf(" daca vreti sa parasiti jocul. \n");

	char caracter;
	switch (caracter=_getch())
	{
	case UNU: {mapa.linii = 9;mapa.coloane = 9;break;}
	case DOI: {mapa.linii = 16;mapa.coloane = 16;break;}
	case TREI: {mapa.linii = 40;mapa.coloane = 40;break;}
	case PATRU: {dificultateCustom();break;}
	case ESC:
	{
		setCursor(6, mapa.linii * 2 + 13);
		printf("\t ");
		coloreaza(142);
		exit(0);
		break;
	}
	default:
	{
		system("cls");
		coloreaza(142);
		printf("\n\t Nu ati ales corect dificultatea! Incercati din nou!\n");
		
		alegeDificultate();
	}
	}
}

int main()
{
	fullScreen();
	do
	{
		system("cls");
		gameOver = 0;
		retry = 0;
		win = 0;
		alegeDificultate();
		initializareMapa();
		generareHarta();
		afisare();
		miscare();

	} while (retry == 1 && (gameOver == 1 || win == 1));


	//for (int i = 100;i <= 200;i++)
	//{
	//coloreaza(i);
	//cout << i<<" aaaaaaaaaaaa"<<"\n";
	//}

	/*while (1)
	{
		char c;
		c = _getch();
		cout << int(c) << "   ";
	}*/

	return 0;
}

