#include "WndObject.h"
#include "Desktop.h"


BEGIN_NAMESPACE(WndDesign)


WndObject::~WndObject() {
	if (is_mouse_tracked) { desktop.LoseTrack(); }
	if (is_mouse_captured) { desktop.LoseCapture(); }
	if (is_on_focus) { desktop.LoseFocus(); }
	if (ime_aware) { desktop.RemoveImeWnd(*this); }
}

void WndObject::SetCapture() { desktop.SetCapture(*this); }
void WndObject::ReleaseCapture() { desktop.ReleaseCapture(*this); }
void WndObject::SetFocus() { desktop.SetFocus(*this); }
void WndObject::PassMouseMsg(MouseMsg msg) { desktop.PassMouseMsg(*this, msg); }
void WndObject::PassKeyMsg(KeyMsg msg) { desktop.PassKeyMsg(*this, msg); }


END_NAMESPACE(WndDesign)