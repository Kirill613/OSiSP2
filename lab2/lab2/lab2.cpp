#include <windows.h>
#include "Table.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Table::LoadDataFromFile("values.txt");

    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Table",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void Paint(HWND hwnd) {
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);

    HDC hBufferDC = CreateCompatibleDC(ps.hdc);
    HBITMAP hBufferBitmap = CreateCompatibleBitmap(ps.hdc, clientRect.right, clientRect.bottom);
    HGDIOBJ hOldBufferBitmap = SelectObject(hBufferDC, hBufferBitmap);

    FillRect(hBufferDC, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    Table::Draw(hBufferDC, clientRect);

    BitBlt(ps.hdc, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);

    SelectObject(hBufferDC, hOldBufferBitmap);
    DeleteObject(hBufferDC);
    DeleteObject(hBufferBitmap);

    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        Paint(hwnd);
        return 0;
    }  

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}