
#include <cstdio>

#include <Windows.h>

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    int status = 0;

    LPCTSTR lpClassName = L"Allocator";

    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
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

    MSG msg = {};
    while(true)
    {
        BOOL ret = GetMessage(&msg, nullptr, 0, 0);

        if (ret > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            ret = (status == 0) ? msg.wParam : -1;
            if(ret != 0)
            {
                printf("an error occured in the event loop\n");
            }

            break;
        }
    }

    return status;
}

LRESULT WINAPI WndProc(_In_ HWND hWnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            break;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
