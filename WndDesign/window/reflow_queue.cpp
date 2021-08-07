#include "reflow_queue.h"
#include "WndObject.h"


BEGIN_NAMESPACE(WndDesign)


ReflowQueue& ReflowQueue::Get() {
	static ReflowQueue reflow_queue;
	return reflow_queue;
}


void ReflowQueue::Commit() {
	for (auto wnd : queue) {
		wnd->UpdateLayout();
	}
	queue.clear();
}


END_NAMESPACE(WndDesign)