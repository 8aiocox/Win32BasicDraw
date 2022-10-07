#pragma once

#include "framework.h"
#include "resource.h"
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#define MAX_LOADSTRING 100
#define ID_CREATE_ECLIPSE 4401
#define ID_CREATE_TRIANGLE 4402
#define ID_CREATE_LINE 4403
#define ID_CREATE_RECTANGLE 4404

#define ID_FILL_COLOR_RED 4410
#define ID_FILL_COLOR_BLUE 4411
#define ID_FILL_COLOR_GREEN 4412

// Global Variables:
HINSTANCE hInst; // current instance
WCHAR szTitle[MAX_LOADSTRING] = TEXT("BasicWindowClass"); // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING] = TEXT("New Window"); // the main window class name

template <class T> void SafeRelease(T** ppT) {
    if (*ppT) {
        (*ppT)->Release();
        *ppT = NULL;
    }
}
enum DrawShape { NoShape, RectangleShape, LineShape, EclipseShape };
enum FillColor { Red, Blue, Green };

template <class T> class DrawnShape {
public:
    D2D1_COLOR_F FillColor;
    D2D1_COLOR_F StrokeColor;
    FLOAT StrokeWidth;
    DrawShape ShapeType;
    T Shape;
    int ID;
    int DisplayIndex;
    DrawnShape(T _shape, D2D1_COLOR_F _fillColor, D2D1_COLOR_F _strokeColor, FLOAT _strokeWidth = 1.0f) { Shape = _shape; FillColor = _fillColor; StrokeColor = _strokeColor;  StrokeWidth = _strokeWidth; }
};


template<class T> class BaseWindow {
public:
protected:
    virtual PCWSTR ClassName() const = 0;
    virtual LRESULT CALLBACK HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;
public:
    BaseWindow() : m_hwnd(NULL) {}
    HWND Window() const { return m_hwnd; }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        T* pThis = NULL;
        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (T*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
            pThis->m_hwnd = hwnd;
        }
        else {
            pThis = (T*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis) {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    //
    //  PURPOSE: Registers the window class.
    //
    ATOM RegisterBasicWindowClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BASICDRAW));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BASICDRAW);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&wcex);
    }

    //
    //   PURPOSE: Saves instance handle and creates main window
    //
    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
    {
        hInst = hInstance; // Store instance handle in our global variable
        RegisterBasicWindowClass(hInstance);
        HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, this);
        if (!hWnd) { return FALSE; }

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }
};