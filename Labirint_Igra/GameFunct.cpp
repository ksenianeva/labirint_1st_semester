#include "framework.h"
#include "Game.h"
#include <stdio.h>
#define _CRT_NO_WARNINGS_
#define MAX_NUM_RECORDS 10
#define wall 2

#define N 15
#define M 15

int levelLoaded = 0; //флаг загрузки уровня
extern int ScreenCase; //для передачи переменной из Labirint cpp
char name[20];

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
	}; //рисунок короны


	HPEN hPen;
	hPen = CreatePen(PS_SOLID, 2, RGB(128,0, 128));
	SelectObject(hdc, hPen);
	Polyline(hdc, p, 8);
	DeleteObject(hPen);
}

int startTime = 0;

void DrawField(HDC hdc) { //загрузка поля из файла
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
		SYSTEMTIME stb;
		GetLocalTime(&stb);
		startTime = stb.wHour * 3600 + stb.wMinute * 60 + stb.wSecond;
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
		

		while (j < M) { //заполняем поле цветом
			RECT rect = { j * sizeX,i * sizeY,  (j + 1) * sizeX,(i + 1) * sizeY };
			if (a[i][j] == 0) {
				FillRect(hdc, &rect, WayBrush);
			}
			else if (a[i][j] == wall) 
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

int endTime = 0;

void setPlayerName(char nameIn[20]) {
	strcpy(name, nameIn);
}

void InsertRecord(char name[], int endTime);

void WinScreen(HDC hdc) { //Функция вызова экрана победы

	SYSTEMTIME stb;
	GetLocalTime(&stb);
	endTime = stb.wHour * 3600 + stb.wMinute * 60 + stb.wSecond - startTime; //после конца уровня считаем сколько на него понадобилось времени
	InsertRecord(name, endTime);

	HBRUSH Background;
	Background = CreateSolidBrush(RGB(100, 200, 213));
	SelectObject(hdc, Background);
	RECT Ground = { 0, 0, 450, 508 };
	FillRect(hdc, &Ground, Background);
	DeleteObject(Background);

	HFONT hFont;
	hFont = CreateFont(32, 0, 0, 0, 900, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"Courier New"
	);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(240, 128, 128));

	TCHAR  string1[] = _T("Уровень пройден!");
	TextOut(hdc, 70, 50, (LPCWSTR)string1, _tcslen(string1));
	TCHAR  stringCount[] = _T("Уровней осталось:0");//Переменная подсчета пройденных уровней
	TextOut(hdc, 50, 150, (LPCWSTR)stringCount, _tcslen(stringCount));
	
	HFONT mFont;
	mFont = CreateFont(28, 0, 0, 0, 900, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"Courier New"
	);
	SelectObject(hdc, mFont);
	SetTextColor(hdc, RGB(240, 128, 128)); //Уменьшаю шрифт под окно
	TCHAR  string2[] = _T("Для продолжения нажмите пробел");
	TextOut(hdc, 10, 100, (LPCWSTR)string2, _tcslen(string2));

}



struct Record {
	char name[20];
	int endTime;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
};

struct Record records[MAX_NUM_RECORDS + 1];
int numRecords = 0;

int CompareRecords(int index1, int index2)
{
	if (records[index1].endTime > records[index2].endTime)
		return -1;
	if (records[index1].endTime < records[index2].endTime)
		return +1;

	return 0;

}


void saveRecords();

void InsertRecord(char name[], int endTime)
{
	strcpy(records[numRecords].name, name);
	records[numRecords].endTime = endTime;

	SYSTEMTIME st;
	// Получаем текущее время
	GetLocalTime(&st);

	// и разбрасываем его по полям в таблицу рекордов
	records[numRecords].year = st.wYear;
	records[numRecords].month = st.wMonth;
	records[numRecords].day = st.wDay;

	records[numRecords].hour = st.wHour;
	records[numRecords].minute = st.wMinute;
	records[numRecords].second = st.wSecond;
		// хороший результат
	int i = numRecords;
	while (i > 0) {
		if (CompareRecords(i - 1, i) < 0) {
			struct Record temp = records[i];
			records[i] = records[i - 1];
			records[i - 1] = temp;
		}
		i--;
	}
	// Если таблица заполнена не полностью
	if (numRecords < MAX_NUM_RECORDS)
		numRecords++;
	saveRecords();
}

void RecordScreen(HDC hdc) {
	HFONT hFont;
	hFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"Courier New"
	);
	SelectObject(hdc, hFont);
	SetTextColor(hdc, RGB(0, 64, 64));

	TCHAR  string1[] = _T("! No ! Дата       ! Время    ! Имя             ! Время игры !");
	TextOut(hdc, 10, 50, (LPCWSTR)string1, _tcslen(string1));

	int i;
	for (i = 0; i < numRecords; i++) {
		TCHAR  string2[80];
		char str[80];
		sprintf(str, "! %2d ! %02d.%02d.%04d ! %02d:%02d:%02d ! %-20s ! %4d ! ",
			i + 1,
			records[i].day, records[i].month, records[i].year,
			records[i].hour, records[i].minute, records[i].second,
			records[i].name, records[i].endTime
		);
		OemToChar(str, string2);
		TextOut(hdc, 10, 24 * (i + 1) + 50, (LPCWSTR)string2, _tcslen(string2));
	}
	DeleteObject(hFont);
}



void moveLeft(HWND hWnd) { //передвижение налево
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

void moveRight(HWND hWnd) { //передвижение направо
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


void moveUp(HWND hWnd) { //передвижение вверх
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

void moveDown(HWND hWnd) { //передвижение вверх
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
	RECT Ground = { 0, 0, 450, 508 };
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

	TCHAR  string2[] = _T("Перед началом введите имя");
	TextOut(hdc, 20, 100, (LPCWSTR)string2, _tcslen(string2));

	TCHAR  string3[] = _T("для сохранения рекорда");
	TextOut(hdc, 50, 150, (LPCWSTR)string3, _tcslen(string3));
	}


void saveProgress(){ //сохранение прогресса в файл

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

void loadProgress(HWND hWnd) { //загрузка прогресса из файла

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

void saveRecords() { //сохранение рекордов в файл

	FILE* saveRec = fopen("Levels\\Records.txt", "wt");
	int i;
	fprintf(saveRec, "%d\n", numRecords);
	for (i = 0; i < numRecords; i++) {
		fprintf(saveRec, "%d %d %d %d %d %d %s %d \n",
			records[i].day, records[i].month, records[i].year,
			records[i].hour, records[i].minute, records[i].second,
			records[i].name, records[i].endTime
		);
	}

	fclose(saveRec);
}

void loadRecords() { //загрузка рекордов из файла

	FILE* loadRec = fopen("Levels\\Records.txt", "rt");
	int i;
	fscanf(loadRec, "%d", &numRecords);
	for (i = 0; i < numRecords; i++) {
		fscanf(loadRec, "%d %d %d %d %d %d %s %d",
			&records[i].day, &records[i].month, &records[i].year,
			&records[i].hour, &records[i].minute, &records[i].second,
			&records[i].name, &records[i].endTime
		);
	}

	fclose(loadRec);
}
