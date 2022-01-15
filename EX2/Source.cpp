#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include "Header.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR WinName[] = _T("MainFrame");

HWND hWnd, hExit, hPaint, hGroupBox, hPen1, hPen2, hPen3;

int APIENTRY _tWinMain(HINSTANCE This, HINSTANCE Prev, LPTSTR cmd, int mode) {
    WNDCLASS wc;
    MSG msg;

    wc.hInstance = This;
    wc.lpszClassName = WinName;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_CROSS);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    if (!RegisterClass(&wc)) return 0;

    hWnd = CreateWindow(WinName,
        _T("Ðåäàêòîð"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT,
        HWND_DESKTOP,
        NULL,
        This,
        NULL);

    if (!hWnd)
    {
        return FALSE;
    }

   hExit = CreateWindow(_T("button"), _T("Âûéòè"),
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        35, 400,
        100, 20,
        hWnd,
        (HMENU)ID_Exit,
        This, NULL);

   hPaint = CreateWindow(WinName, _T(""),
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        180, 40,
        920, 480,
        hWnd,
        (HMENU)ID_Paint,
        This, NULL);

    hGroupBox = CreateWindow(_T("button"), _T("Pens"),
        WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
        20, 40,
        130, 190,
        hWnd,
        (HMENU)ID_GroupBox,
        This, NULL);

    hPen1 = CreateWindow(_T("button"), _T("Êðàñíûé"),
        WS_GROUP | WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        30, 80,
        100, 20,
        hWnd,
        (HMENU)ID_Pen1,
        This, NULL);

    hPen2 = CreateWindow(_T("button"), _T("Ñèíèé"),
         WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        30, 130,
        100, 20,
        hWnd,
        (HMENU)ID_Pen2,
        This, NULL);

    hPen3 = CreateWindow(_T("button"), _T("Çåë¸íûé"),
        WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
        30, 180,
        100, 20,
        hWnd,
        (HMENU)ID_Pen3,
        This, NULL);

    ShowWindow(hWnd, mode);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdñ;
    static HPEN hPen;
    static COLORREF color;
    static bool status = false;
    static POINT t;
    WORD wmEvent, wmId;

    switch (message)
    {
    case WM_CREATE:
        color = RGB(0, 0, 0);
        break;

    case WM_INITDIALOG: 
        status = false;
        return TRUE;
        break;

    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        switch (wmId)
        {
        case BN_CLICKED:
        {
        case ID_Pen1:
            if (SendDlgItemMessage(hWnd, ID_Pen1, BM_GETCHECK, 0, 0))
                color = RGB(255, 0, 0);
            else
                color = RGB(0, 0, 0);
            break;

        case ID_Pen2:
            if (SendDlgItemMessage(hWnd, ID_Pen2, BM_GETCHECK, 0, 0))
                color = RGB(0, 0, 255);
            else
                color = RGB(0, 0, 0);
                break;

        case ID_Pen3:
            if (SendDlgItemMessage(hWnd, ID_Pen3, BM_GETCHECK, 0, 0))
                color = RGB(0, 255, 0);
            else
                color = RGB(0, 0, 0);
                break;
        }
        break;

        case ID_Exit:
            DestroyWindow(hWnd);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    //case WM_PAINT:
    //    hdñ = BeginPaint(hPaint, &ps);
    //    UpdateWindow(hPaint);
    //    EndPaint(hPaint, &ps);
    //    break;

    case WM_LBUTTONUP:
        if (status) {
            hdñ = GetDC(hPaint);
            MoveToEx(hdñ, t.x, t.y, NULL);
            LineTo(hdñ, LOWORD(lParam), HIWORD(lParam));
            ReleaseDC(hPaint, hdñ);
        }
        status = FALSE;
        break;

    case WM_LBUTTONDOWN:
        status = TRUE;
        t.x = LOWORD(lParam);
        t.y = HIWORD(lParam);
        break;

    case WM_MOUSEMOVE:
        if (status) {
            hdñ = GetDC(hPaint);
            hPen = CreatePen(PS_SOLID, 3, color);
            SelectObject(hdñ, hPen);
            MoveToEx(hdñ, t.x, t.y, NULL);
            LineTo(hdñ, t.x = LOWORD(lParam), 
                t.y = HIWORD(lParam));
            ReleaseDC(hPaint, hdñ);
        }
        break;

    //case WM_SIZE:
    //    InvalidateRect(hPaint, NULL, FALSE);
    //    break;

    case WM_DESTROY:
        DeleteObject(hPen);
        PostQuitMessage(0); 
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}