#pragma once

#include "../geometry/point.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

using std::wstring;

using HANDLE = void*;


void ImeEnable(HANDLE hwnd);
void ImeDisable(HANDLE hwnd);
void ImeSetPosition(HANDLE hwnd, Point point);
void ImeUpdateString(HANDLE hwnd, uint type);

std::wstring ImeGetString();
size_t ImeGetCursorPosition();


END_NAMESPACE(WndDesign)