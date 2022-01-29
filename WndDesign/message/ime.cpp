#include "ime.h"
#include "../window/desktop.h"


BEGIN_NAMESPACE(WndDesign)


void ImeApi::ImeEnable() { desktop.ImeEnable(wnd, *this); }

void ImeApi::ImeDisable() { desktop.ImeDisable(wnd); }

void ImeApi::ImeSetPosition(Point point) { desktop.ImeSetPosition(wnd, point); }


END_NAMESPACE(WndDesign)