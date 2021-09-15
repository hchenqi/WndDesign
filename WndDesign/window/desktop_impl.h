#pragma once

#include "Desktop.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


class DesktopImpl : public Desktop {
private:
	using frame_ptr = std::unique_ptr<DesktopFrame>;
	std::vector<frame_ptr> frame_list;

public:
	virtual void AddChild(frame_ptr frame) override { 
		frame_list.push_back(std::move(frame));
		frame_list.back()->Show();
	}
	virtual void RemoveChild(DesktopFrame& frame) override { 
		auto it = std::find_if(frame_list.begin(), frame_list.end(), [&](const frame_ptr& ptr) { return ptr.get() == &frame; });
		if (it == frame_list.end()) { throw std::invalid_argument("invalid desktop frame"); }
		frame_list.erase(it);
	}

public:
	const Size GetSize() {}

private:
	std::vector<ref_ptr<WndObject>> track_wnd_list;
	ref_ptr<WndObject> wnd_capture;
	ref_ptr<WndObject> wnd_focus;


public:

public:
	virtual void MessageLoop() override {}
	virtual void Terminate() override {}
};

extern DesktopImpl& desktop_impl;


END_NAMESPACE(WndDesign)