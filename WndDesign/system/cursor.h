#pragma once

#include "../geometry/geometry.h"


BEGIN_NAMESPACE(WndDesign)


enum class CursorStyle : uint {
	Hide = 0,
	Arrow = 32512,			// IDC_ARROW
	Hand = 32649,			// IDC_HAND
	Text = 32513,		    // IDC_IBEAM
	Move = 32646,			// IDC_SIZEALL
	Help = 32651,			// IDC_HELP
	Crosshair = 32515,		// IDC_CROSS
	Wait = 32514,			// IDC_WAIT
	Progress = 32650,		// IDC_APPSTARTING
	No = 32648,				// IDC_NO
	Resize0 = 32644,		// IDC_SIZEWE
	Resize45 = 32643,		// IDC_SIZENESW
	Resize90 = 32645,		// IDC_SIZENS
	Resize135 = 32642,	    // IDC_SIZENWSE
	Default = Arrow,
};


void SetCursor(CursorStyle cursor = CursorStyle::Default);

Point GetCursorPosition();


END_NAMESPACE(WndDesign)