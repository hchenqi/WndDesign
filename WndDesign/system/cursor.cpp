#include "cursor.h"

#include <Windows.h>


BEGIN_NAMESPACE(WndDesign)


void SetCursor(Cursor cursor) {
	::SetCursor(LoadCursorW(NULL, MAKEINTRESOURCE(cursor)));
}


END_NAMESPACE(WndDesign)