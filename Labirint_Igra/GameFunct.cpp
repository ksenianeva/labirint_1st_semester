#include "framework.h"
#include "Game.h"
#include <stdio.h>
#define _CRT_NO_WARNINGS_

#define wall 2

#define N 15
#define M 15

int levelLoaded = 0; //флаг загрузки уровня
extern int ScreenCase;

struct PlayerPlace { //структура игрока
	int x;
	int y;
};

PlayerPlace player = { 0, 0 };

int a[N][M];
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
	hPen = CreatePen(PS_SOLID, 2, RGB(128,0, 128));
	SelectObject(hdc, hPen);
	Polyline(hdc, p, 8);
	DeleteObject(hPen);
}

void DrawField(HDC hdc) {
	if (!levelLoaded) {
		FILE* Testlevel = fopen("Levels\\TestLevel.txt", "rt");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				fscanf(Testlevel, "%d", &a[i][j]);
			}

		}

		fscanf(Testlevel, "%d", &player.x);
		fscanf(Testlevel, "%d", &player.y);
		fclose(Testlevel);
		levelLoaded = 1;
	}

	HBRUSH WallBrush;
	WallBrush = CreateSolidBrush(RGB(128, 0, 128));

	HBRUSH WayBrush;
	WayBrush = CreateSolidBrush(RGB(240, 128, 128));

	HBRUSH GamerBrush;
	GamerBrush = CreateSolidBrush(RGB(100, 200, 213));

	if (player.x == 0 && player.y == 0) {
		player.x = 1;
		player.y = 1;
	}

	RECT plRect = { player.x * sizeX, player.y * sizeY, (player.x + 1) * sizeX, (player.y + 1) * sizeY };
	FillRect(hdc, &plRect, GamerBrush);


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
	if (a[player.y][player.x-1] == wall) {
		return;
	} //чек на стену

	a[player.y][player.x] = 0;
	player.x--;

	

	if (a[player.y][player.x] == 1) {
		ScreenCase = 2;
		InvalidateRect(hWnd, NULL, TRUE);
		return;
	}//чек на корону

	a[player.y][player.x] = 3;
	
}

void moveRight(HWND hWnd) {
	if (a[player.y][player.x + 1] == wall) {
		return;
	} //чек на стену

	a[player.y][player.x] = 0;
	player.x++;

	if (a[player.y][player.x] == 1) {
		ScreenCase = 2;
		InvalidateRect(hWnd, NULL, TRUE);
		return;
	} //чек на корону

	a[player.y][player.x] = 3;

}


void moveUp(HWND hWnd) {
	if (a[player.y - 1][player.x] == wall) {
		return;
	} //чек на стену

	a[player.y][player.x] = 0;
	player.y--;

	if (a[player.y][player.x] == 1) {
		ScreenCase = 2;
		InvalidateRect(hWnd, NULL, TRUE);
		return;
	}//чек на корону

	a[player.y][player.x] = 3;

}

void moveDown(HWND hWnd) {
	if (a[player.y + 1][player.x] == wall) {
		return;
	} //чек на стену

	a[player.y][player.x] = 0;
	player.y++;

	if (a[player.y][player.x] == 1) {
		ScreenCase = 2;
		InvalidateRect(hWnd, NULL, TRUE);
		return;
	}//чек на корону

	a[player.y][player.x] = 3;

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


void saveProgress(){

	FILE* savePath = fopen("Levels\\Save.txt", "wt");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			fprintf(savePath, "%d ", a[i][j]);
		}
		fprintf(savePath, "\n");
	}

	fprintf(savePath, "%d %d", player.x, player.y);

	fclose(savePath);
}

void loadProgress(HWND hWnd) {

	FILE* savePath = fopen("Levels\\Save.txt", "rt");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			fscanf(savePath, "%d", &a[i][j]);
		}

	}

	fscanf(savePath, "%d", &player.x);
	fscanf(savePath, "%d", &player.y);
	fclose(savePath);
	InvalidateRect(hWnd, NULL, TRUE);
}