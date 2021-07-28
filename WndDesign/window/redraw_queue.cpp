#include "redraw_queue.h"


BEGIN_NAMESPACE(WndDesign)


RedrawQueue& RedrawQueue::Get() {
	static RedrawQueue redraw_queue;
	return redraw_queue;
}


void RedrawQueue::Commit() {
	while (!redraw_queue.empty()) {
		WndObject& wnd = *redraw_queue.front(); redraw_queue.pop_front();
		if (wnd.HasParent() && wnd.HasInvalidRegion()) {
			wnd.GetParent().InvalidateChildRegion(wnd);
		}
	}

	Target::BeginDraw();
	for (auto frame : commit_queue) { frame->UpdateInvalidRegion(); }
	try {
		Target::EndDraw();
	} catch (std::runtime_error&) {

	}

	for (auto frame : commit_queue) { frame->Present(); }
	commit_queue.clear();
}


END_NAMESPACE(WndDesign)