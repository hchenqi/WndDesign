#include "ime.h"
#include "../window/desktop.h"


BEGIN_NAMESPACE(WndDesign)


ImeMsgHandler::ImeMsgHandler(WndObject& wnd) : wnd(wnd) { desktop.SetImeWnd(wnd, *this); }

void ImeMsgHandler::ImeEnable() { desktop.ImeEnable(wnd); }

void ImeMsgHandler::ImeDisable() { desktop.ImeDisable(wnd); }

void ImeMsgHandler::ImeSetPosition(Point point) { desktop.ImeSetPosition(wnd, point); }


END_NAMESPACE(WndDesign)