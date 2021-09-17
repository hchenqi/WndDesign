#include "desktop.h"
#include "../frame/DesktopFrame.h"
#include "../system/win32_api.h"


BEGIN_NAMESPACE(WndDesign)

Desktop desktop;


void Desktop::AddChild(frame_ptr frame) {
	RegisterChild(*frame);
	frame_list.push_back(std::move(frame));
	frame_list.back()->Show();
}

void Desktop::RemoveChild(frame_ref frame) {
	auto it = std::find_if(frame_list.begin(), frame_list.end(), [&](const frame_ptr& ptr) { return ptr.get() == &frame; });
	if (it == frame_list.end()) { throw std::invalid_argument("invalid desktop frame"); }
	frame_list.erase(it);
}

const Size Desktop::GetSize() const { return Win32::GetDesktopSize(); }

DesktopFrame& Desktop::GetDesktopFrame(WndObject& wnd) {
	ref_ptr<WndObject> child = &wnd;
	for (ref_ptr<WndObject> parent = child->parent; parent != &desktop; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("window has no desktop frame"); }
	}
	return static_cast<DesktopFrame&>(*child);
}

DesktopFrame& Desktop::GetDesktopFramePoint(WndObject& wnd, Point& point) {
	ref_ptr<WndObject> child = &wnd;
	for (ref_ptr<WndObject> parent = child->parent; parent != &desktop; child = parent, parent = child->parent) {
		if (parent == nullptr) { throw std::invalid_argument("window has no desktop frame"); }
		point += parent->GetChildOffset(*child);
	}
	return static_cast<DesktopFrame&>(*child);
}


END_NAMESPACE(WndDesign)