#include <Windows.h>
#include <windowsx.h>

#include "ApplicationFractal.h"

constexpr RHINO::BackendAPI RHI_BACKEND_API = RHINO::BackendAPI::D3D12;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    LPCTSTR lpzClass = TEXT("My Window Class!");

    if (!RegMyWindowClass(hInstance, lpzClass))
        return 1;

    const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

    HWND hWnd = CreateWindow(lpzClass, TEXT("Dialog Window"),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             //x, y
                             0, 0,
                             WINDOW_WIDTH, WINDOW_HEIGHT,
                             nullptr, nullptr,
                             hInstance, nullptr);
    if (!hWnd) return 2;

    ApplicationFractal application{};
    application.Init(RHI_BACKEND_API);
    application.InitSwapchain(hInstance, hWnd);

    MSG msg = {nullptr};
    int iGetOk = 0;

    bool applicationShouldClose = false;
    while (!applicationShouldClose) {
        if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) {
                applicationShouldClose = true;
            }
        }
        else {
            application.Logic();
        }
    }
    application.ReleaseSwapchain();
    application.Release();
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
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams));
    }
    LONG_PTR userPtr = GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message) {

        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}