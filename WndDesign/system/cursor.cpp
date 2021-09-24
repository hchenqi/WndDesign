#include "cursor.h"

#include <Windows.h>


BEGIN_NAMESPACE(WndDesign)


void SetCursor(Cursor cursor) {
	::SetCursor(LoadCursorW(NULL, MAKEINTRESOURCE(cursor)));
}

Point GetCursorPosition() {
	POINT point; ::GetCursorPos(&point);
	return Point(point.x, point.y);
}


END_NAMESPACE(WndDesign)