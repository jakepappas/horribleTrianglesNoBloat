#include <windows.h>
#include "engine.h"
#include <cstdint>

// Define your screen size
constexpr int WIDTH = 800;
constexpr int HEIGHT = 800;
// Create a pixel buffer
uint32_t pixels[WIDTH * HEIGHT];

// Forward declaration of window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
// Register window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = reinterpret_cast<LPCSTR>(L"MyWindowClass");

    RegisterClass(&wc);

// Create the window
    HWND hwnd = CreateWindowEx(
            0,
            wc.lpszClassName,
            reinterpret_cast<LPCSTR>(L"Pixel Renderer"),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
            nullptr, nullptr, hInstance, nullptr
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

// Create BITMAPINFO header
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = WIDTH;
    bmi.bmiHeader.biHeight = -HEIGHT;  // Negative to indicate top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

// Game/render loop
    MSG msg = {};
    std::unique_ptr<Engine> engine = std::make_unique<Engine>(pixels);
    while (true) {
// Handle messages
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) return 0;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        engine->Render();

// Get device context and draw the buffer
        HDC hdc = GetDC(hwnd);
        StretchDIBits(
                hdc,
                0, 0, WIDTH, HEIGHT,
                0, 0, WIDTH, HEIGHT,
                pixels,
                &bmi,
                DIB_RGB_COLORS,
                SRCCOPY
        );
        ReleaseDC(hwnd, hdc);
        Sleep(16);
    }
}

// Basic Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
