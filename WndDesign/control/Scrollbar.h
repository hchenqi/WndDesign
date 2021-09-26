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
	Scrollbar(ScrollFrame& frame, uint width = 20) : frame(frame), width(width) { RegisterChild(slider); }

private:
	ScrollFrame& frame;

	// layout
private:
	uint width;
	uint height = length_min;
	uint slider_height = 0;
	int slider_offset = 0;
private:
	bool IsShown() const { return slider_height < height; }
	Size GetSize() const { return Size(IsShown() ? width : 0, height); }
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override { height = size_ref.height; return GetSize(); }
public:
	void UpdateScrollOffset(uint content_height, uint frame_height, int frame_offset) {
		bool is_shown = IsShown();
		slider_height = (uint)roundf((float)height * frame_height / content_height);
		slider_offset = (int)roundf((float)height * frame_offset / content_height);
		if (is_shown ^ IsShown()) { SizeUpdated(GetSize()); }
	}
private:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (Interval(slider_offset, slider_height).Contains(point.y)) { return &slider; }
		return this;
	}

	// paint
private:
	static constexpr Color frame_color = Color::Silver;
	static constexpr Color slider_color_normal = Color::DarkGray;
	static constexpr Color slider_color_hover = Color::Gray;
	static constexpr Color slider_color_press = Color::DimGray;
private:
	Color slider_color = slider_color_normal;
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		if (IsShown()) {
			figure_queue.add(point_zero, new Rectangle(GetSize(), frame_color));
			figure_queue.add(Point(0, slider_offset), new Rectangle(Size(width, slider_height), slider_color));
		}
	}

	// message
private:


	// slider
private:
	struct Slider : public WndObject {
	private:
		Scrollbar& GetScrollbar() const { return static_cast<Scrollbar&>(GetParent()); }
	private:
		enum class State { Normal, Hover, Press } state = State::Normal;
	private:
		void SetColor(Color color) { GetScrollbar().slider_color = color; Redraw(region_infinite); }
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
			case MouseMsg::LeftDown: SetState(State::Press); break;
			case MouseMsg::Move: if (state == State::Press) {

			}break;
			case MouseMsg::LeftUp: SetState(State::Hover); break;
			}
		}
		virtual void OnNotifyMsg(NotifyMsg msg) override {
			switch (msg) {
			case NotifyMsg::MouseEnter: SetState(State::Hover); break;
			case NotifyMsg::MouseLeave: SetState(State::Normal); break;
			}
		}
	};

	Slider slider;
};


END_NAMESPACE(WndDesign)