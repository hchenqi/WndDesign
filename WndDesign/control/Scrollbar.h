#pragma once

#include "../frame/ScrollFrame.h"
#include "../geometry/interval.h"
#include "../figure/shape.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class Scrollbar;


template<>
class Scrollbar<Vertical> : public WndType<Auto, Assigned> {
public:
	using ScrollFrame = ScrollFrame<Vertical>;

public:
	Scrollbar(float width = 20.0f) : width(width) { RegisterChild(slider); }

	// frame
private:
	ref_ptr<ScrollFrame> frame = nullptr;
private:
	ScrollFrame& GetScrollFrame() { if (frame == nullptr) { throw std::invalid_argument("invalid scroll frame"); } return *frame; }
public:
	void SetScrollFrame(ScrollFrame& frame) { this->frame = &frame; UpdateScrollOffset(); }

	// layout
private:
	float width;
	float height = 0.0f;
	float slider_height = 0.0f;
	float slider_offset = 0.0f;
private:
	bool IsShown() const { return slider_height < height; }
	Size GetSize() const { return Size(IsShown() ? width : 0.0f, height); }
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override { height = size_ref.height; return GetSize(); }
public:
	void UpdateScrollOffset() {
		bool is_shown = IsShown(); ScrollFrame& frame = GetScrollFrame();
		slider_height = height * frame.GetFrameLength() / frame.GetChildLength();
		slider_offset = height * frame.GetFrameOffset() / frame.GetChildLength();
		if (is_shown ^ IsShown()) { SizeUpdated(GetSize()); }
		Redraw(region_infinite);
	}
private:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (Interval(slider_offset, slider_height).Contains(point.y)) { return &slider; }
		return this;
	}

	// paint
private:
	static constexpr Color frame_color = Color::DarkGray;
	static constexpr Color slider_color_normal = Color::Gray;
	static constexpr Color slider_color_hover = Color::DimGray;
	static constexpr Color slider_color_press = 0x606060;
private:
	Color slider_color = slider_color_normal;
private:
	void SetSliderColor(Color color) { slider_color = color; Redraw(region_infinite); }
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		if (IsShown()) {
			figure_queue.add(point_zero, new Rectangle(GetSize(), frame_color));
			figure_queue.add(Point(0.0f, slider_offset), new Rectangle(Size(width, slider_height), slider_color));
		}
	}

	// message
private:
	float mouse_down_offset = 0.0f;
private:
	void OnMousePress(float y) { mouse_down_offset = y - slider_offset; }
	void OnMouseDrag(float y) { GetScrollFrame().UpdateFrameOffset((y - mouse_down_offset) * GetScrollFrame().GetChildLength() / height); }
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { GetScrollFrame().Scroll((float)-msg.wheel_delta); }
	}

private:
	struct Slider : public WndObject {
	private:
		Scrollbar& GetScrollbar() const { return static_cast<Scrollbar&>(GetParent()); }
	private:
		enum class State { Normal, Hover, Press } state = State::Normal;
	private:
		void SetColor(Color color) { GetScrollbar().SetSliderColor(color); }
		void SetState(State state) {
			this->state = state;
			switch (state) {
			case State::Normal: SetColor(slider_color_normal); break;
			case State::Hover: SetColor(slider_color_hover); break;
			case State::Press: SetColor(slider_color_press); break;
			}
		}
	private:
		virtual void OnMouseMsg(MouseMsg msg) override {
			switch (msg.type) {
			case MouseMsg::LeftDown: SetState(State::Press); GetScrollbar().OnMousePress(msg.point.y); SetCapture(); break;
			case MouseMsg::Move: if (state == State::Press) { GetScrollbar().OnMouseDrag(msg.point.y); }break;
			case MouseMsg::LeftUp: SetState(State::Hover); ReleaseCapture(); break;
			case MouseMsg::WheelVertical: GetScrollbar().OnMouseMsg(msg); break;
			}
		}
		virtual void OnNotifyMsg(NotifyMsg msg) override {
			switch (msg) {
			case NotifyMsg::MouseEnter: SetState(State::Hover); break;
			case NotifyMsg::MouseLeave: SetState(State::Normal); break;
			}
		}
	}slider;
};


END_NAMESPACE(WndDesign)