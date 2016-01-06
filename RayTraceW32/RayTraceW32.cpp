// RayTraceW32.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RayTraceW32.h"

#include "bitmap.h"
#include "camera.h"
#include "scene.h"
#include "kernel.h"
#include "engine.h"

#define MAX_LOADSTRING 100

#define TILE_SIZE 5
#define WIDTH_TILES 200
#define HEIGHT_TILES 150

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

EngineSettings* settings = new EngineSettings();
HWND hWindow;

Scene* scene = new Scene();
Engine* renderingEngine = nullptr;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
COLORREF getColorRef(RGBType rgb);
void renderScene();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_RAYTRACEW32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RAYTRACEW32));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_RAYTRACEW32));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RAYTRACEW32);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, TILE_SIZE*WIDTH_TILES, TILE_SIZE*HEIGHT_TILES, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	hWindow = hWnd;

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	hWindow = hWnd;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_SAVEIMAGE:
			if (renderingEngine) {
				renderingEngine->saveOutput();
			}
			break;
		case IDM_RERENDER:
			renderScene();
			break;
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
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		LPRECT updateRect;

		if (renderingEngine == nullptr) {
			break;
		}

		hdc = BeginPaint(hWnd, &ps);

		int xFrom = ps.rcPaint.left;
		int xTo = ps.rcPaint.right;
		int yFrom = ps.rcPaint.top;
		int yTo = ps.rcPaint.bottom;

		for (int y = yFrom; y < yTo; y++) {
			for (int x = xFrom; x < xTo; x++) {
				if (x >= WIDTH_TILES * TILE_SIZE || y >= HEIGHT_TILES * TILE_SIZE) break;

				RGBType pixelRgb = renderingEngine->rendererBuffer[(y * WIDTH_TILES * TILE_SIZE) + x];
				COLORREF pixelColor = getColorRef(pixelRgb);
				SetPixel(hdc, x, y, pixelColor);
			}
		}
		EndPaint(hWnd, &ps);
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

// Message handler for about box.
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



void renderScene() {

	scene->getCamera()->setPosition(Vect(0, 0, 22));
	scene->getCamera()->lookAt(Vect(100, 0, 22));
	scene->getCamera()->setFocalLength(35);
	scene->getCamera()->setSensorSize(35);
	scene->getCamera()->setDepthOfField(2.0);

	scene->getCamera()->focusAt(Vect(40, 0, 22)); //scene->getCamera()->setFocusDistance(25);
	scene->getCamera()->setupCamera();

	settings->minRaysPerPixel = 1;
	settings->maxRayPerPixel = 2;
	settings->maxBounces = 1;
	settings->threads = 2;
	settings->xTiles = WIDTH_TILES;
	settings->yTiles = HEIGHT_TILES;
	settings->tileSize = TILE_SIZE;
	settings->maxVariance = 0.0000001;

	if (renderingEngine == nullptr) {
		renderingEngine = new Engine(settings, scene);
	}

	renderingEngine->Reload();
	renderingEngine->render();
}


void displayTile(int tileId) {
	int xT = tileId % WIDTH_TILES;
	int yT = (tileId - xT) / WIDTH_TILES;
	int xFrom = xT * TILE_SIZE;
	int yFrom = yT * TILE_SIZE;
	int xTo = xFrom + TILE_SIZE;
	int yTo = yFrom + TILE_SIZE;
	const LPRECT rect = new RECT({ xFrom,yFrom,xTo,yTo });


	InvalidateRect(hWindow, rect, false);

	/*
	HDC hdc = GetDC(hWindow);
	for (int y = yFrom; y < yTo; y++) {
		for (int x = xFrom; x < xTo; x++) {
			int bufferIndex = (y * WIDTH_TILES * TILE_SIZE) + x;
			RGBType pixelRgb = renderingEngine->rendererBuffer[bufferIndex];
			COLORREF pixelColor = getColorRef(pixelRgb);
			SetPixel(hdc, x, y, pixelColor);
		}
	}
	ReleaseDC(hWindow, hdc);
	*/
}

inline COLORREF getColorRef(RGBType rgb) {
	return RGB(rgb.r * 255, rgb.g * 255, rgb.b * 255);
}