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

int nrMine;

void afisare();
void initializareMapa();
void miscare();
void generareHarta();
void dificultateCustom();
void alegeDificultate();

void initializareMapa()
{
	mapa.matrice[0][0].valoare = '+';
	mapa.matrice[0][0].demascat = true;
	mapa.matrice[mapa.linii*2+2][0].valoare = '+';
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
		mapa.matrice[0][i].valoare = '_';
		mapa.matrice[0][i].demascat = true;
		mapa.matrice[2 * mapa.linii + 2][i].valoare = '_';
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
	bool gameOver = 0;
	int x = 6, y = 6;
	mapa.matrice[x*2][y*2-1].valoare = '<';
	mapa.matrice[x*2][y*2+1].valoare = '>';
	
	while (1)
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
					x--;
				}
				break;
			}
			case ENTER:
			{
				if (mapa.matrice[x * 2][y * 2].valoare == 'M')
				{
					mapa.matrice[x * 2][y * 2].valoare = 'B';
					system("cls");
					printf("\n\t GAME OVER - You just hit a mine");
					gameOver = 1;
				}
				mapa.matrice[x * 2][y * 2].demascat = true;
				break;
			}
			case SPACE:
			{
				mapa.matrice[x * 2][y * 2].steag = 1;
				break;
			}
			case BACKSPACE:
			{
				if (mapa.matrice[x * 2][y * 2].steag == 1)
				{
					mapa.matrice[x * 2][y * 2].steag = 0;
				}
			}
			case ESC:
			{
				printf("\n");
				exit(0);
			}
		}
		if(gameOver==0)
		afisare();
	}
}

void numeroteazaVecinii(int x, int y)
{
	if (y - 1 > 0 && x - 1 > 0)
	{
		if (strchr("M+_|", mapa.matrice[(x - 1) * 2][(y - 1) * 2].valoare) == 0)
			mapa.matrice[(x - 1) * 2][(y - 1) * 2].valoare++;
	}
	if (x - 1 > 0)
	{
		if (strchr("M+_|", mapa.matrice[(x - 1) * 2][y * 2].valoare) == 0)
			mapa.matrice[(x - 1) * 2][y * 2].valoare++;
	}
	if (y <= mapa.coloane&& x - 1>0)
	{
		if (strchr("M+_|", mapa.matrice[(x - 1) * 2][(y + 1) * 2].valoare) == 0)
			mapa.matrice[(x - 1) * 2][(y + 1) * 2].valoare++;
	}
	if (y <= mapa.coloane)
	{
		if (strchr("M+_|", mapa.matrice[x * 2][(y + 1) * 2].valoare) == 0)
			mapa.matrice[x * 2][(y + 1) * 2].valoare++;
	}
	if (x <= mapa.linii && y<=mapa.coloane)
	{
		if (strchr("M+_|", mapa.matrice[(x + 1) * 2][(y + 1) * 2].valoare) == 0)
			mapa.matrice[(x + 1) * 2][(y + 1) * 2].valoare++;
	}
	if (x <= mapa.linii)
	{
		if (strchr("M+_|", mapa.matrice[(x + 1) * 2][y * 2].valoare)==0)
			mapa.matrice[(x + 1) * 2][y * 2].valoare++;
	}
	if(x<=mapa.linii&&y-1>0)
	{
		if (strchr("M+_|", mapa.matrice[(x + 1) * 2][(y - 1) * 2].valoare) == 0)
			mapa.matrice[(x + 1) * 2][(y - 1) * 2].valoare++;
	}
	if (y - 1>0)
	{
		if (strchr("M+_|", mapa.matrice[x * 2][(y - 1) * 2].valoare) == 0)
			mapa.matrice[x * 2][(y - 1) * 2].valoare++;
	}
}

void generareHarta()
{
	int x, y, i = 0;

	if(mapa.linii+mapa.coloane<=20)
		nrMine = (mapa.linii + mapa.coloane);
	else if(mapa.linii+mapa.coloane<=40)
		nrMine = (mapa.linii + mapa.coloane)+10;
	else
		nrMine = (mapa.linii + mapa.coloane)*2;

	srand(time(nullptr));
	while (i < nrMine)
	{
		x = rand() % (mapa.linii*2)+1;
		y = rand() % (mapa.coloane*2)+1;
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

	printf("\n\t Utilizati tastele Up,Down,Left and Right pentru a misca cursorul.");
	printf("\n\t Utilizati tast Enter pentru a descoperi casutele, Space pentru a plasa");
	printf("\n\t steaguri in locurile in care presupuneti ca sunt Mine si BackSpace pentru");
	printf("\n\t a sterge steagurile si pentru a reveni la forma initiala.\n\n");

	for (int i = 0;i <= mapa.linii * 2 + 2;i++)
	{
		printf("\t");
		for (int j = 0;j <= mapa.coloane * 2 + 2;j++)
		{
			if (mapa.matrice[i][j].demascat == true || strchr("+_|<>", mapa.matrice[i][j].valoare)!=0)
				printf("%c", mapa.matrice[i][j].valoare);
			else if (mapa.matrice[i][j].steag == 1)
				printf("S");
			else
				printf("X");
		}
		printf("\n");
	}
}

void dificultateCustom()
{
	int linii, coloane;
	system("cls");
	printf("\v\t Introduceti doua valori aflate intre 9 si 80 pentru a seta dimensiunile\n");
	scanf_s("%d", &linii);
	scanf_s("%d", &coloane);
	if (linii < 9 || linii>80 || coloane < 9 || coloane>80)
	{
		system("cls");
		printf("\v\t Ati introdus valori incorecte. Sa mai incercam odata!\n");
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
	int alegere;

	printf("\v\n\t 1. Easy 9x9");
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
	alegeDificultate();
	initializareMapa();
	generareHarta();
	afisare();
	miscare();
}