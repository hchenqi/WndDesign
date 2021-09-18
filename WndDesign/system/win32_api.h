#pragma once

#include "../geometry/geometry.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

using std::wstring;

using HANDLE = void*;


BEGIN_NAMESPACE(Win32)


const Size GetDesktopSize();

HANDLE CreateWnd(Rect region, const wstring& title);
void DestroyWnd(HANDLE hwnd);
void SetWndUserData(HANDLE hwnd, void* data);

void SetWndTitle(HANDLE hwnd, const wstring& title);
void SetWndRegion(HANDLE hwnd, Rect region);

void ShowWnd(HANDLE hwnd);
void MinimizeWnd(HANDLE hwnd);
void MaximizeWnd(HANDLE hwnd);
void RestoreWnd(HANDLE hwnd);

void InvalidateWndRegion(HANDLE hwnd, Rect region);

void SetCapture(HANDLE hwnd);
void ReleaseCapture();
void SetFocus(HANDLE hwnd);

int MessageLoop();
void Terminate();


END_NAMESPACE(Win32)

END_NAMESPACE(WndDesign)