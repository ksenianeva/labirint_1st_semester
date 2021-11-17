#include "framework.h"
#include "Game.h"
#include <stdio.h>

#define N 15
#define M 15


//Field
int a[N][M] = {
{ 2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2 },
{ 2, 3, 2, 0, 0,   0, 2, 2, 0, 0,   0, 0, 0, 2, 2 },
{ 2, 0, 0, 0, 2,   0, 2, 0, 0, 0,   0, 0, 2, 2, 2 },
{ 2, 2, 2, 2, 2,   0, 0, 0, 0, 0,   0, 2, 0, 0, 2 },
{ 2, 0, 2, 0, 2,   0, 2, 2, 0, 2,   0, 2, 0, 0, 2 },
{ 2, 0, 0, 0, 0,   2, 2, 0, 0, 0,	0, 0, 0, 0, 0 },
{ 2, 0, 2, 2, 0,   2, 0, 2, 0, 0,	0, 2, 2, 2, 2 },
{ 2, 2, 2, 0, 0,   0, 0, 0, 0, 2,   0, 2, 0, 0, 1 },
{ 2, 0, 0, 0, 0,   2, 2, 2, 0, 2,   0, 2, 0, 0, 2 },
{ 2, 0, 0, 0, 0,   2, 0, 0, 0, 2,   0, 2, 2, 0, 2 },
{ 2, 0, 0, 0, 0,   0, 0, 0, 2, 2,   0, 0, 0, 0, 2 },
{ 2, 0, 2, 2, 0,   2, 0, 0, 0, 2,   0, 2, 2, 0, 0 },
{ 2, 2, 2, 0, 0,   2, 2, 0, 0, 0,   0, 2, 0, 0, 2 },
{ 2, 0, 0, 0, 0,   2, 0, 2, 0, 0,   0, 2, 0, 0, 2 },
{ 2, 2, 2, 2, 2,   2, 2, 2, 2, 2,   2, 2, 2, 2, 2 }
};

//Путь - 0
//Выход с короной - 1
//Стена - 2
//Игрок - 3




int sizeX = 30;
int sizeY = 30;


void Crown(HDC hdc, int cx, int cy, int sizeX, int sizeY, COLORREF color) {
	POINT p[] = {
		cx,		cy - sizeY,
		cx + sizeX / 2,	cy,
		cx + sizeX,	cy - sizeY,
		cx + sizeX,	cy + sizeY,
		cx - sizeX,	cy + sizeY,
		cx - sizeX,	cy - sizeY,
		cx - sizeX / 2,	cy,
		cx,		cy - sizeY
	};

	HPEN hPen;
	hPen = CreatePen(PS_SOLID, 3, color);
	SelectObject(hdc, hPen);
	Polyline(hdc, p, 8);
	DeleteObject(hPen);
}

void DrawField(HDC hdc) {
	HBRUSH WallBrush;
	WallBrush = CreateSolidBrush(RGB(128, 0, 128));

	HBRUSH WayBrush;
	WayBrush = CreateSolidBrush(RGB(240, 128, 128));

	HBRUSH GamerBrush;
	GamerBrush = CreateSolidBrush(RGB(255, 228, 225));

	int i, j;
	i = 0;
	while (i < N) {
		j = 0;
		

		while (j < M) {
			RECT rect = { j * sizeX,i * sizeY,  (j + 1) * sizeX,(i + 1) * sizeY };
			if (a[i][j] == 0) {
				FillRect(hdc, &rect, WayBrush);
			}
			else if (a[i][j] == 2) //wall
			{
				FillRect(hdc, &rect, WallBrush);
			}
			else if (a[i][j] == 3) {
				FillRect(hdc, &rect, GamerBrush);
			}
			else if (a[i][j] == 1) {
				int x = i;
				int y = j;
				FillRect(hdc, &rect, WayBrush);
				Crown(hdc, x*62, y*16 , 10, 10, RGB(0,0,0));
				
			}
			else {

			}
			j++;
		}
		i++;
	}
}

void moveLeft() {
	int i, j;
	i = 0;
	while (i < N) {
		j = 1;
		while (j < M) {
			if (a[i][j] == 3) {
				if (a[i][j - 1] == 0) {
					a[i][j - 1] = 3;
					a[i][j] = 0;
				
				}
				else if (a[i][j - 1] == 1) {
					a[i][j - 1] = 3;
					a[i][j] = 0;
				}
			}
			j++;
		}
		i++;
	}
}

void moveRight() {
	int i = 0;
	while (i < N) {
		int j = M - 2;
		while (j >= 0) {
			if (a[i][j] == 3) {
				if (a[i][j + 1] == 0) {
					a[i][j + 1] = 3;
					a[i][j] = 0;
					
				}

				else if (a[i][j + 1] == 1) {
					a[i][j + 1] = 1; // Crown "eats" a player, next level
					a[i][j] = 0;
					
				}
			}
			j--;
		}
		i++;
	}
}


void moveUp() {
	int i = 1;
	while (i < N) {
		int j = 0;
		while (j < M) {
			if (a[i][j] == 3) {
				if (a[i - 1][j] == 0) {
					a[i - 1][j] = 3;
					a[i][j] = 0;

				}
				else if (a[i - 1][j] == 1) {
					a[i - 1][j] = 3;
					a[i][j] = 0;
					
				}
			}
			j++;
		}
		i++;
	}
}

void moveDown() {
	int i = N - 2;
	while (i >= 0) {
		int j = 0;
		while (j < M) {
			if (a[i][j] == 3) {
				if (a[i + 1][j] == 0) {
					a[i + 1][j] = 3;
					a[i][j] = 0;
				
				}
				else if (a[i + 1][j] == 1) {
					a[i + 1][j] = 3;
					a[i][j] = 0;
					
				}
			}
			j++;
		}
		i--;
	}
}