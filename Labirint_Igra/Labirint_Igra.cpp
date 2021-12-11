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
      CW_USEDEFAULT, 0, 700, 508, nullptr, nullptr, hInstance, nullptr);

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
    static HWND nameBtn; // дескриптор кнопки забрать имя
    static HWND hEdt1; // дескрипторы поля редактирования
    static HWND StartBtn; // дескриптор кнопки старта
    static HWND RecordBtn; // дескриптор кнопки рекордов

    switch (message)
    {
    case WM_MOUSEMOVE: //подключение мыши
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
        if (lParam == (LPARAM) StartBtn)    // если нажали на кнопку Play
        {
            ScreenCase = 0; //экран игры 
            SetFocus(hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        else if (lParam == (LPARAM)RecordBtn)    // если нажали на кнопку Рекорд
        {
        ScreenCase = 3; //экран игры 
        SetFocus(hWnd);
        InvalidateRect(hWnd, NULL, TRUE);
        }
        else if(lParam == (LPARAM)nameBtn){
            TCHAR StrT[20];
            char str[20];

            // Берем имя 
            GetWindowText(hEdt1, StrT, sizeof(StrT));
            wcstombs(str, StrT, 20);

            // Фокус возвращаем в игру
            // нажатия клавиш снова управляют игрой!
            SetFocus(hWnd);
            setPlayerName(str);
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
                DrawField(hdc);
                break;
            case 1:
                MenuScreen(hdc); //отображение окна меню
                break;
            case 2:
                WinScreen(hdc); //экран победы 
                break;
            case 3:
                RecordScreen(hdc); //экран рекордов
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
        //статичный цвет кнопки
        else
            FillRect(Item->hDC, &Item->rcItem, (HBRUSH)GetStockObject(WHITE_BRUSH));

        //вывести текст
        int len = GetWindowTextLength(Item->hwndItem);
        char* buf = new char[len + 1];
        GetWindowTextA(Item->hwndItem, buf, len + 1);
        DrawTextA(Item->hDC, buf, len, &Item->rcItem, DT_CENTER);

        return true;
    }

    case WM_CREATE:
            loadRecords();
        // Создаем и показываем кнопки
            //Кнопка Play
            StartBtn = CreateWindowW(_T("button"), _T("Play!"), 
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_OWNERDRAW,  //создаем кнопку самостоятельно
            500, 100, 150, 40, hWnd, 0, hInst, NULL);
            ShowWindow(StartBtn, SW_SHOWNORMAL);
        

            //Кнопка таблица рекордов
            RecordBtn = CreateWindowW(_T("button"), _T("Рекорды"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_OWNERDRAW,  //создаем кнопку самостоятельно
            500, 200, 150, 40, hWnd, 0, hInst, NULL);
            ShowWindow(RecordBtn, SW_SHOWNORMAL);

            hInst = ((LPCREATESTRUCT)lParam)->hInstance; // дескриптор приложения
        // Создаем и показываем поле редактирования 
            hEdt1 = CreateWindowW(_T("edit"), _T("Noname"),
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT, 500, 50, 150, 30,
                hWnd, 0, hInst, NULL);
            ShowWindow(hEdt1, SW_SHOWNORMAL);

           //Кнопка запомнить имя
            nameBtn = CreateWindowW(_T("button"), _T("Запомнить!"),
                WS_CHILD | WS_VISIBLE | WS_BORDER | BS_OWNERDRAW,  //создаем кнопку самостоятельно
                500, 10, 150, 40, hWnd, 0, hInst, NULL);
            ShowWindow(nameBtn, SW_SHOWNORMAL);
        
            
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
        case 0x53: //s
            saveProgress();
            break;
        case 0x4C: //l
            loadProgress(hWnd);
            break;
        case 0x20: //пробел
            ScreenCase = 1;
            InvalidateRect(hWnd, NULL, TRUE);
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
