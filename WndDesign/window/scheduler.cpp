#include "scheduler.h"
#include "Desktop.h"
#include "../system/d2d_target.h"


BEGIN_NAMESPACE(WndDesign)


class Scheduler {
public:
	static ReflowQueue reflow_queue;
	static RedrawQueue redraw_queue;
};

ReflowQueue& GetReflowQueue() {
	return Scheduler::reflow_queue;
}

RedrawQueue& GetRedrawQueue() {
	return Scheduler::redraw_queue;
}

void CommitQueue() {
	Scheduler::reflow_queue.Commit();
	Scheduler::redraw_queue.Commit();
}


void ReflowQueue::Commit() {
	for()


	while (!reflow_queue.empty()) {
		WndObject& wnd = *reflow_queue.front(); reflow_queue.pop_front();
		if (wnd.IsLayoutInvalid() && wnd.IsSizeAuto()) {
			
		}
		if (wnd.IsSizeInvalid() && wnd.HasParent()) {
			wnd.GetParent().InvalidateChildSize(wnd);
		}
	}
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