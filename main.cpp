
#include <cstdio>

#include <Windows.h>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    int status = 0;

    LPCTSTR lpClassName = L"Allocator";

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = lpClassName;
    wc.hIconSm = nullptr;

    if (RegisterClassEx(&wc) == 0)
    {
        status = -1;
        printf("error: failed to register class\n");
    }

    HWND hWnd = CreateWindowEx(0, lpClassName, L"Allocator", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hWnd, SW_SHOW);

    while (true);

    return status;
}
