#pragma once

#include "mouse_msg.h"
#include "timer.h"
#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


enum class MouseTrackMsg {
	None,
	LeftDown,
	LeftUp,
	MouseMove,
	LeftClick,			   // down - up (move < 5px is also accepted)
	LeftDoubleClick,       // down - up - down(<0.5s)
	LeftTripleClick,       // down - up - down(<0.5s) - up - down(<0.5s)
	LeftDrag,              // down - move
};


class MouseTracker : Uncopyable {
public:
	MouseTracker(WndObject& wnd) :
		wnd(static_cast<WndObjectHelper&>(wnd)), hit_count(0), is_mouse_down(false), mouse_down_position() {
	}
	~MouseTracker() {}
private:
	struct WndObjectHelper : public WndObject { public: WndObject::SetCapture; WndObject::ReleaseCapture; };
	WndObjectHelper& wnd;
private:
	uint hit_count;
	static constexpr uint timer_interval = 500;  // 500ms
	Timer timer = Timer([&]() { hit_count = 0; timer.Stop(); });
public:
	bool is_mouse_down;
	Point mouse_down_position;
private:
	static constexpr uint move_tolerate_range = 5;  // 5px
	static uint Square(int x) { return static_cast<uint>(x * x); };
	static uint SquareDistance(Point a, Point b) { return Square(a.x - b.x) + Square(a.y - b.y); }
public:
	MouseTrackMsg Track(MouseMsg msg) {
		MouseTrackMsg ret;
		switch (msg.type) {
		case MouseMsg::LeftDown:
			switch (hit_count) {
			case 1: ret = MouseTrackMsg::LeftDoubleClick; break;
			case 2: ret = MouseTrackMsg::LeftTripleClick; break;
			case 0: default: ret = MouseTrackMsg::LeftDown; hit_count = 0; break;
			}
			if (SquareDistance(msg.point, mouse_down_position) > Square(move_tolerate_range)) {
				ret = MouseTrackMsg::LeftDown; hit_count = 0;
			}
			hit_count++; timer.Set(timer_interval);
			is_mouse_down = true;
			mouse_down_position = msg.point;
			wnd.SetCapture();
			break;
		case MouseMsg::LeftUp:
			ret = is_mouse_down ? MouseTrackMsg::LeftClick : MouseTrackMsg::LeftUp;
			is_mouse_down = false;
			wnd.ReleaseCapture();
			break;
		case MouseMsg::Move:
			ret = is_mouse_down ? MouseTrackMsg::LeftDrag : MouseTrackMsg::MouseMove;
			break;
		default: 
			ret = MouseTrackMsg::None; 
			break;
		}
		return ret;
	}
};


END_NAMESPACE(WndDesign)