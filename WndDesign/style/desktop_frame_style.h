#pragma once

#include "style.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)


struct DesktopFrameStyle {
	Style::LengthStyle width, height;
	Style::PositionStyle position;
	Style::BorderStyle border;
	std::wstring title;
};


END_NAMESPACE(WndDesign)