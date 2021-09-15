#pragma once

#include "../frame/DesktopFrame.h"


BEGIN_NAMESPACE(WndDesign)


struct Desktop {
	virtual void AddChild(std::unique_ptr<DesktopFrame> frame) pure;
	virtual void RemoveChild(DesktopFrame& frame) pure;
	virtual void MessageLoop() pure;
	virtual void Terminate() pure;
};

extern Desktop& desktop;


END_NAMESPACE(WndDesign)