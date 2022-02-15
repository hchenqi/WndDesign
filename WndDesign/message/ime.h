#pragma once

#include "../geometry/point.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

class WndObject;


struct Ime {
	static void Enable(WndObject& wnd);
	static void Disable(WndObject& wnd);
	static void SetPosition(WndObject& wnd, Point point);
	static std::wstring GetString();
};

constexpr Ime ime;


END_NAMESPACE(WndDesign)