#pragma once

#include "../common/core.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)

class WndObject;


class ReflowQueue {
private:
	std::vector<ref_ptr<WndObject>> queue;
	ReflowQueue() {}
public:
	static ReflowQueue& Get();
	void AddWnd(WndObject& wnd) { queue.push_back(&wnd); }
	void RemoveWnd(WndObject& wnd) { queue.erase(std::find(queue.begin(), queue.end(), &wnd)); }
	void Commit();
};

inline ReflowQueue& GetReflowQueue() { return ReflowQueue::Get(); }


END_NAMESPACE(WndDesign)