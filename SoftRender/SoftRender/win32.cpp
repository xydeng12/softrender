#include "win32.h"
#include <cassert>

Window *win = nullptr;
static long long int GetTimerFrequency();

static LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        win->close = true;
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
        win->button[0] = true;
        win->mouse.orbitPos = GetMousePos();
        break;
    case WM_LBUTTONUP:
        win->button[0] = false;
        break;
    case WM_RBUTTONDOWN:
        win->button[1] = true;
        win->mouse.orbitPos = GetMousePos();
        break;
    case WM_RBUTTONUP:
        win->button[1] = false;
        break;
    case WM_MOUSEWHEEL:
        win->mouse.wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
        break;
    case WM_MOUSEMOVE:
        TrackMouseEvent(&win->tme);
        break;
    case WM_MOUSELEAVE:
        win->button[0] = false;
        win->button[1] = false;
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static void InitBitmapInfoHeader(BITMAPINFOHEADER& bmInfo, int width, int height)
{
    memset(&bmInfo, 0, sizeof(BITMAPINFOHEADER));
    bmInfo.biSize = sizeof(BITMAPINFOHEADER);
    bmInfo.biWidth = width;
    bmInfo.biHeight = -height;
    bmInfo.biPlanes = 1;
    bmInfo.biBitCount = 32;
    bmInfo.biCompression = BI_RGB;
    bmInfo.biSizeImage = width * height * 4;
}

void InitWindow(int width, int height)
{
    win = (Window*)malloc(sizeof(Window));
    memset(win, 0, sizeof(Window));
    win->close = false;
    win->width = width;
    win->height = height;
    win->tme.cbSize = sizeof(win->tme);
    win->tme.dwFlags = TME_LEAVE;
    win->tme.hwndTrack = win->hWnd;
    win->tme.dwHoverTime = 0;
    win->timerFreq = GetTimerFrequency();

    // Register the window class.
    WNDCLASS wc = { };
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"SoftRender_Window";

    RegisterClass(&wc);

    // Create the window.
    win->hWnd = CreateWindow(L"SoftRender_Window", L"SoftRender",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        0, 0, 0, 0, NULL, NULL, GetModuleHandle(NULL), NULL);
    assert(win->hWnd != NULL);

    HDC hDC = GetDC(win->hWnd);
    win->memDC = CreateCompatibleDC(hDC);
    ReleaseDC(win->hWnd, hDC);

    BITMAPINFOHEADER bmInfo;
    InitBitmapInfoHeader(bmInfo, width, height);
    LPVOID pvBits;
    win->bmDIB = CreateDIBSection(win->memDC, (BITMAPINFO*)&bmInfo, DIB_RGB_COLORS, &pvBits, NULL, NULL);
    assert(win->bmDIB != NULL);

    win->bmOld = (HBITMAP)SelectObject(win->memDC, win->bmDIB);
    win->wndFrameBuf = (unsigned char*)pvBits;

    RECT rect = { 0, 0, width, height };//一个矩形范围 左上右下
    int wx, wy, sx, sy;
    AdjustWindowRect(&rect, GetWindowLong(win->hWnd, GWL_STYLE), 0);//调整窗口大小
    wx = rect.right - rect.left;
    wy = rect.bottom - rect.top;
    sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2; // GetSystemMetrics(SM_CXSCREEN)获取你屏幕的分片率
    sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2; //我的是1920*1080， 这里是为了算出中心位置
    //printf("%d %d %d\n", sx, GetSystemMetrics(SM_CXSCREEN), wx);
    //printf("%d %d %d\n", sy, GetSystemMetrics(SM_CYSCREEN), wy);
    if (sy < 0) sy = 0;

    SetWindowPos(win->hWnd, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
    SetForegroundWindow(win->hWnd);

    ShowWindow(win->hWnd, SW_SHOW);

    HandleWindowMessage();

    memset(win->wndFrameBuf, 0, width * height * 4);
}

void HandleWindowMessage()
{
    MSG msg = { };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else break;
    }
}

void DestroyWindow()
{
    if (win->memDC)
    {
        if (win->bmOld)
        {
            SelectObject(win->memDC, win->bmOld);
            win->bmOld = NULL;
        }
        DeleteDC(win->memDC);
        win->memDC = NULL;
    }
    if (win->bmDIB)
    {
        DeleteObject(win->bmDIB);
        win->bmDIB = NULL;
    }
    if (win->hWnd)
    {
        CloseWindow(win->hWnd);
        win->hWnd = NULL;
    }

    free(win);
}

void DrawWindow(unsigned char* buf)
{
    for (int i = 0; i < win->height; ++i)
        for (int j = 0; j < win->width; ++j)
        {
            int idx = (i * win->width + j) * 4;

            win->wndFrameBuf[idx] = buf[idx + 2];
            win->wndFrameBuf[idx + 1] = buf[idx + 1];
            win->wndFrameBuf[idx + 2] = buf[idx];
            win->wndFrameBuf[idx + 3] = buf[idx + 3];
        }

    HDC hDC = GetDC(win->hWnd);
    BitBlt(hDC, 0, 0, win->width, win->height, win->memDC, 0, 0, SRCCOPY);
    ReleaseDC(win->hWnd, hDC);
}

static long long int GetTimerValue()
{
    LARGE_INTEGER value;
    QueryPerformanceCounter(&value);
    return value.QuadPart;
}

static long long int GetTimerFrequency()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}

double GetTime()
{
    return (double)(GetTimerValue() / win->timerFreq);
}

vec2 GetMousePos()
{
    POINT point;
    GetCursorPos(&point);
    ScreenToClient(win->hWnd, &point);
    return vec2((double)point.x, (double)point.y);
}

