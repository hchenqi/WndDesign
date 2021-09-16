#include "WndObject.h"
#include "Desktop.h"


BEGIN_NAMESPACE(WndDesign)


void WndObject::SetCapture() { desktop.SetCapture(*this); }
void WndObject::ReleaseCapture() { desktop.ReleaseCapture(*this); }
void WndObject::SetFocus() { desktop.SetFocus(*this); }
void WndObject::PassMouseMsg(MouseMsg msg) { desktop.PassMouseMsg(*this, msg); }
void WndObject::PassKeyMsg(KeyMsg msg) { desktop.PassKeyMsg(*this, msg); }


END_NAMESPACE(WndDesign)