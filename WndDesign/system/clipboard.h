#pragma once

#include "../common/core.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

using std::wstring;


void SetClipboardData(const wstring& text);

void GetClipboardData(wstring& text);


END_NAMESPACE(WndDesign)