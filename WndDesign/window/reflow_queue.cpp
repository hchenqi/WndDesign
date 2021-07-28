#include "reflow_queue.h"


BEGIN_NAMESPACE(WndDesign)


ReflowQueue& ReflowQueue::Get() {
	static ReflowQueue reflow_queue;
	return reflow_queue;
}


void ReflowQueue::Commit() {
	for (auto wnd : reflow_queue) {
		wnd->
	}


	while (!reflow_queue.empty()) {
		WndObject& wnd = *reflow_queue.front(); reflow_queue.pop_front();
		if (wnd.IsLayoutInvalid() && wnd.IsSizeAuto()) {

		}
		if (wnd.IsSizeInvalid() && wnd.HasParent()) {
			wnd.GetParent().InvalidateChildSize(wnd);
		}
	}
}


END_NAMESPACE(WndDesign)