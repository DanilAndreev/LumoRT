#include <Windows.h>
#include <windowsx.h>

#include "mainLogic.h"

extern HWND g_RHINO_HWND;
extern HINSTANCE g_RHINO_HInstance;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);


    LPCTSTR lpzClass = TEXT("My Window Class!");

    if (!RegMyWindowClass(hInstance, lpzClass))
        return 1;

    const int WINDOW_WIDTH = screenWidth, WINDOW_HEIGHT = screenHeight;

    HWND hWnd = CreateWindow(lpzClass, TEXT("Dialog Window"),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             //x, y
                             0, 0,
                             WINDOW_WIDTH, WINDOW_HEIGHT,
                             nullptr, nullptr,
                             hInstance, nullptr);
    if (!hWnd) return 2;

    MSG msg = {nullptr};
    int iGetOk = 0;
    while ((iGetOk = GetMessage(&msg, nullptr, 0, 0)) != 0) {
        if (iGetOk == -1) return 3;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName) {
    WNDCLASS wcWindowClass = {0};
    wcWindowClass.lpfnWndProc = (WNDPROC) WndProc;
    wcWindowClass.style = CS_HREDRAW | CS_VREDRAW;
    wcWindowClass.hInstance = hInst;
    wcWindowClass.lpszClassName = lpzClassName;
    wcWindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcWindowClass.hbrBackground = (HBRUSH) COLOR_APPWORKSPACE;
    return RegisterClass(&wcWindowClass);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_CREATE) {
        SetWindowLongPtr(hWnd, GWLP_USERDATA,
                         reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams));
    }
    // auto *game = reinterpret_cast<Core::Win32GameRoot*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message) {
        case WM_CREATE:
            break;
        case WM_PAINT:
            break;
        case WM_SIZE:
            g_RHINO_HWND = hWnd;
            g_RHINO_HInstance = GetModuleHandle(NULL);
            mainLogic();
            break;
        case WM_KEYDOWN:
            break;
        case WM_KEYUP:
            break;
        case WM_MOUSEMOVE:
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}