#pragma once

#include "../common/core.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)

class DesktopWndFrame;


class RedrawQueue {
private:
	std::vector<ref_ptr<DesktopWndFrame>> queue;
	RedrawQueue() {}
public:
	static RedrawQueue& Get();
	void AddDesktopFrame(DesktopWndFrame& frame) { queue.push_back(&frame); }
	void RemoveDesktopFrame(DesktopWndFrame& frame) { queue.erase(std::find(queue.begin(), queue.end(), &frame)); }
	void Commit();
};

inline RedrawQueue& GetRedrawQueue() { return RedrawQueue::Get(); }


END_NAMESPACE(WndDesign)