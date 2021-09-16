#pragma once

#include "../geometry/geometry.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

using std::wstring;

using HANDLE = void*;


BEGIN_NAMESPACE(Win32)


const Size GetDesktopSize();

HANDLE CreateWnd(Rect region, const wstring& title);
void DestroyWnd(HANDLE hWnd);
void SetWndUserData(HANDLE hWnd, void* data);

void SetWndTitle(HANDLE hWnd, const wstring& title);
void SetWndRegion(HANDLE hWnd, Rect region);

void ShowWnd(HANDLE hWnd);
void MinimizeWnd(HANDLE hWnd);
void MaximizeWnd(HANDLE hWnd);
void RestoreWnd(HANDLE hWnd);

void InvalidateWndRegion(HANDLE hWnd, Rect region);

void SetCapture(HANDLE hWnd);
void ReleaseCapture();
void SetFocus(HANDLE hWnd);

int MessageLoop();
void Terminate();


END_NAMESPACE(Win32)

END_NAMESPACE(WndDesign)