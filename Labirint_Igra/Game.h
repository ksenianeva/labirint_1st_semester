#pragma once

void DrawField(HDC hdc);
void moveDown(HWND hWnd);
void moveUp(HWND hWnd);
void moveLeft(HWND hWnd);
void moveRight(HWND hWnd);
void MenuScreen(HDC hdc);
void WinScreen(HDC hdc);
void saveProgress();
void loadProgress(HWND hWnd);
void loadRecords();
void RecordScreen(HDC hdc);
void setPlayerName(char nameIn[20]);