#include "cursor.h"

#include <Windows.h>


BEGIN_NAMESPACE(WndDesign)


void SetCursor(Cursor cursor) {
	if (cursor == Cursor::Default) { return; }
	::SetCursor(LoadCursorW(NULL, MAKEINTRESOURCE(cursor)));
}


END_NAMESPACE(WndDesign)