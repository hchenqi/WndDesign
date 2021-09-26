#include "ime.h"
#include "../window/desktop.h"


BEGIN_NAMESPACE(WndDesign)


ImeApi::ImeApi(ref_ptr<WndObject> wnd) : wnd(*wnd) {
	if (wnd == nullptr) { throw std::invalid_argument("invalid window pointer"); }
	desktop.SetImeWnd(*wnd, *this);
}

void ImeApi::ImeEnable() { desktop.ImeEnable(wnd); }

void ImeApi::ImeDisable() { desktop.ImeDisable(wnd); }

void ImeApi::ImeSetPosition(Point point) { desktop.ImeSetPosition(wnd, point); }


END_NAMESPACE(WndDesign)