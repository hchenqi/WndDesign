#pragma once

#include "../common/core.h"

#include <deque>
#include <vector>


BEGIN_NAMESPACE(WndDesign)

class WndObject;
class DesktopWndFrame;


class ReflowQueue {
private:
	friend class Scheduler;
	std::vector<ref_ptr<WndObject>> reflow_queue;
	ReflowQueue() {}
public:
	void AddWnd(WndObject& wnd) { reflow_queue.push_back(&wnd); }
	void RemoveWnd(WndObject& wnd) { reflow_queue.erase(std::find(reflow_queue.begin(), reflow_queue.end(), &wnd)); }
	void Commit();
};


class RedrawQueue {
private:
	friend class Scheduler;
	std::deque<ref_ptr<WndObject>> redraw_queue;
	std::vector<ref_ptr<DesktopWndFrame>> commit_queue;
	RedrawQueue() {}
public:
	void AddWnd(WndObject& wnd) { redraw_queue.push_back(&wnd); }
	void RemoveWnd(WndObject& wnd) { redraw_queue.erase(std::find(redraw_queue.begin(), redraw_queue.end(), &wnd)); }
	void AddDesktopFrame(DesktopWndFrame& frame) { commit_queue.push_back(&frame); }
	void Commit();
};


ReflowQueue& GetReflowQueue();
RedrawQueue& GetRedrawQueue();


END_NAMESPACE(WndDesign)