// Labirint_Igra.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Labirint_Igra.h"
#include "Game.h"

#define MAX_LOADSTRING 100
#define _CRT_NO_WARNINGS_


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
int ScreenCase = 1;
int xPos;
int yPos;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABIRINTIGRA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABIRINTIGRA));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABIRINTIGRA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LABIRINTIGRA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 465, 508, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND StartBtn; // дескриптор кнопки

    switch (message)
    {
    case WM_MOUSEMOVE:
    {
        switch (wParam)
        {
        case MK_LBUTTON:
            xPos = LOWORD(lParam);
            yPos = HIWORD(lParam);
        }
    }



    case WM_COMMAND:
        {
        if (lParam == (LPARAM) StartBtn)    // если нажали на кнопку
        {
            ScreenCase = 0;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            switch (ScreenCase)
            {
            case 0:
                DestroyWindow(StartBtn); //удаляем кнопку, иначе она будет поверх окна с игрой
                DrawField(hdc);
                break;

            case 1:
                MenuScreen(hdc);
                break;
            case 2:
                WinScreen(hdc);
                break;

            }
           

            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DRAWITEM:
    {
        static HFONT hfontButton = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, L"Courier");
        LPDRAWITEMSTRUCT Item = (LPDRAWITEMSTRUCT)lParam;

        //отображение текста
        SetBkMode(Item->hDC, TRANSPARENT);
        SetTextColor(Item->hDC, RGB(100,200,213));
        SelectObject(Item->hDC, hfontButton);

        //черный фон при клике
        if (Item->itemState & ODS_SELECTED)
            FillRect(Item->hDC, &Item->rcItem, (HBRUSH)GetStockObject(BLACK_BRUSH));
        else
            FillRect(Item->hDC, &Item->rcItem, (HBRUSH)GetStockObject(WHITE_BRUSH));

        //нарисовать текст
        int len = GetWindowTextLength(Item->hwndItem);
        char* buf = new char[len + 1];
        GetWindowTextA(Item->hwndItem, buf, len + 1);
        DrawTextA(Item->hDC, buf, len, &Item->rcItem, DT_CENTER);

        return true;
    }

    case WM_CREATE:
        // Создаем и показываем кнопку
        if (ScreenCase == 1) {
            StartBtn = CreateWindowW(_T("button"), _T("Play!"),
                WS_CHILD | WS_VISIBLE | WS_BORDER | BS_OWNERDRAW,  //создаем кнопку самостоятельно
                110, 100, 220, 50, hWnd, 0, hInst, NULL);
            ShowWindow(StartBtn, SW_SHOWNORMAL);

        }
            
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_DOWN:
            moveDown(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_LEFT:
            moveLeft(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_UP:
            moveUp(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case VK_RIGHT:
            moveRight(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case 0x53:
            saveProgress();
            break;
        case 0x4C:
            loadProgress(hWnd);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
