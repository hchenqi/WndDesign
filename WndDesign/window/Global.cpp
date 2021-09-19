#pragma once

#include "Global.h"
#include "desktop.h"
#include "../frame/DesktopFrame.h"


BEGIN_NAMESPACE(WndDesign)


void Global::AddWnd(std::unique_ptr<DesktopFrame> frame) { desktop.AddChild(std::move(frame));}
void Global::RemoveWnd(DesktopFrame& frame) { desktop.RemoveChild(frame); }
void Global::MessageLoop() { desktop.MessageLoop(); }
void Global::Terminate() { desktop.Terminate(); }


END_NAMESPACE(WndDesign)