#pragma once

#include "Global.h"
#include "desktop.h"
#include "../frame/DesktopFrame.h"


BEGIN_NAMESPACE(WndDesign)


void Global::AddWnd(std::unique_ptr<DesktopFrame> frame) { desktop.AddChild(std::move(frame));}
void Global::AddWnd(alloc_ptr<DesktopFrame> frame) { AddWnd(std::unique_ptr<DesktopFrame>(frame)); }
void Global::RemoveWnd(DesktopFrame& frame) { desktop.RemoveChild(frame); }

DesktopFrame& Global::GetDesktopFrame(WndObject& wnd) { return desktop.GetDesktopFrame(wnd); }
DesktopFrame& Global::GetDesktopFramePoint(WndObject& wnd, Point& point) { return desktop.GetDesktopFramePoint(wnd, point); }

void Global::MessageLoop() { desktop.MessageLoop(); }
void Global::Terminate() { desktop.Terminate(); }


END_NAMESPACE(WndDesign)