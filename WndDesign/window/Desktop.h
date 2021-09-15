#pragma once

#include "wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)

class DesktopWndFrame;


class Desktop final : public WndObject {
public:
	using child_ptr = child_ptr<Relative, Relative>;
	using frame_ptr = std::unique_ptr<DesktopWndFrame>;

private:
	Desktop() {}
	~Desktop() {}
public:
	static Desktop& Get() {}

private:
	std::vector<frame_ptr> frame_list;

public:
	void AddChild(child_ptr child, DesktopWndStyle style) {

	}
	void AddChild(frame_ptr frame) {

	}

private:
	ref_ptr<WndObject> wnd_track;
	ref_ptr<WndObject> wnd_capture;
	ref_ptr<WndObject> wnd_focus;

public:
	void MessageLoop() {}
	void Terminate() {}
};

extern Desktop desktop;


END_NAMESPACE(WndDesign)