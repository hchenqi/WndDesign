#include "ime.h"
#include "../window/desktop.h"


BEGIN_NAMESPACE(WndDesign)


void Ime::Enable(WndObject& wnd) { desktop.ImeEnable(wnd); }

void Ime::Disable(WndObject& wnd) { desktop.ImeDisable(wnd); }

void Ime::SetPosition(WndObject& wnd, Point point) { desktop.ImeSetPosition(wnd, point); }

std::wstring Ime::GetString() {	return desktop.ImeGetString(); }


END_NAMESPACE(WndDesign)