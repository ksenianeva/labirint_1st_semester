#include "framework.h"
#include "Game.h"
#include <stdio.h>

#define N 15
#define M 15

extern int ScreenCase;

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

void WinScreen(HDC hdc) { //Функция вызова экрана победы
	HBRUSH Background;
	Background = CreateSolidBrush(RGB(100, 200, 213));
	SelectObject(hdc, Background);
	RECT Ground = { 0, 0, 1920, 1080 };
	FillRect(hdc, &Ground, Background);
	DeleteObject(Background);

	HFONT hFont;
	hFont = CreateFont(32, 0, 0, 0, 900, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"Courier New"
	);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(124, 252, 0));

	TCHAR  string1[] = _T("Уровень пройден!");
	TextOut(hdc, 50, 50, (LPCWSTR)string1, _tcslen(string1));

	TCHAR  string2[] = _T("Для продолжения нажмите пробел");
	TextOut(hdc, 10, 100, (LPCWSTR)string2, _tcslen(string2));

	TCHAR  stringCount[] = _T("Уровней осталось:");/*Тут должна быть переменная подсчета пройденных уровней*/
	TextOut(hdc, 50, 150, (LPCWSTR)stringCount, _tcslen(stringCount));
	

}

void moveLeft(HWND hWnd) {
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
					ScreenCase = 2;
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			j++;
		}
		i++;
	}
}

void moveRight(HWND hWnd) {
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
					a[i][j + 1] = 1; // Корона "съедает" игрока, следующий уровень
					a[i][j] = 0;
					ScreenCase = 2;
					InvalidateRect(hWnd, NULL, TRUE);
					
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

void MenuScreen(HDC hdc) { //отрисовка главного экрана
	HBRUSH Background;
	Background = CreateSolidBrush(RGB(100, 200, 213));
	SelectObject(hdc, Background);
	RECT Ground = { 0, 0, 1920, 1080 };
	FillRect(hdc, &Ground, Background);
	DeleteObject(Background);

	HFONT hFont;
	hFont = CreateFont(32, 0, 0, 0, 900, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"Courier New"
	);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(100, 200, 213));

	TCHAR  string1[] = _T("Лабиринты.beta");
	TextOut(hdc, 100, 50, (LPCWSTR)string1, _tcslen(string1));
	}

