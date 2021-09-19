#pragma once

#include "Global.h"
#include "desktop.h"
#include "../frame/DesktopFrame.h"
#include "../system/win32_api.h"


BEGIN_NAMESPACE(WndDesign)


void Global::AddWnd(std::unique_ptr<DesktopFrame> frame) { desktop.AddChild(std::move(frame));}
void Global::RemoveWnd(DesktopFrame& frame) { desktop.RemoveChild(frame); }
void Global::MessageLoop() { Win32::MessageLoop(); }
void Global::Terminate() { Win32::Terminate(); }


END_NAMESPACE(WndDesign)