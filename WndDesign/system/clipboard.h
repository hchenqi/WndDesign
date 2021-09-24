#pragma once

#include "../common/core.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)


void SetClipboardData(std::wstring text);

void GetClipboardData(std::wstring& text);


END_NAMESPACE(WndDesign)