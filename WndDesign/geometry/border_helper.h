#pragma once

#include "geometry.h"
#include "../style/cursor_style.h"


BEGIN_NAMESPACE(WndDesign)


enum class BorderPosition : uint {
	None = 0,
	Left = 10,			// HTLEFT
	Top = 12,			// HTTOP
	Right = 11,			// HTRIGHT
	Bottom = 15,		// HTBOTTOM
	LeftTop = 13,		// HTTOPLEFT
	RightTop = 14,		// HTTOPRIGHT
	LeftBottom = 16,	// HTBOTTOMLEFT
	RightBottom = 17	// HTBOTTOMRIGHT
};

inline BorderPosition HitTestBorderPosition(Size window_size, float border_width, Point point) {
	float x1 = border_width, y1 = border_width;
	float x2 = window_size.width - border_width, y2 = window_size.height - border_width;
	float width = window_size.width, height = window_size.height;
	float x = point.x, y = point.y;
	if (x < 0.0f) { return BorderPosition::None; }
	if (x < x1) {
		if (y < 0.0f) { return BorderPosition::None; }
		if (y < y1) { return BorderPosition::LeftTop; }
		if (y < y2) { return BorderPosition::Left; }
		if (y < height) { return BorderPosition::LeftBottom; }
		return BorderPosition::None;
	}
	if (x < x2) {
		if (y < 0.0f) { return BorderPosition::None; }
		if (y < y1) { return BorderPosition::Top; }
		if (y < y2) { return BorderPosition::None; }
		if (y < height) { return BorderPosition::Bottom; }
		return BorderPosition::None;
	}
	if (x < width) {
		if (y < 0.0f) { return BorderPosition::None; }
		if (y < y1) { return BorderPosition::RightTop; }
		if (y < y2) { return BorderPosition::Right; }
		if (y < height) { return BorderPosition::RightBottom; }
		return BorderPosition::None;
	}
	return BorderPosition::None;
}

inline Cursor GetBorderPositionCursor(BorderPosition border_position) {
	switch (border_position) {
	case BorderPosition::Left: return Cursor::Resize0;
	case BorderPosition::Top: return Cursor::Resize90;
	case BorderPosition::Right: return Cursor::Resize0;
	case BorderPosition::Bottom: return Cursor::Resize90;
	case BorderPosition::LeftTop: return Cursor::Resize135;
	case BorderPosition::RightTop: return Cursor::Resize45;
	case BorderPosition::LeftBottom: return Cursor::Resize45;
	case BorderPosition::RightBottom: return Cursor::Resize135;
	default: return Cursor::Default;
	}
}


END_NAMESPACE(WndDesign)