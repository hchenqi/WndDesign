#pragma once

#include "../window/wnd_traits.h"
#include "../geometry/interval.h"
#include "../figure/shape.h"
#include "../system/cursor.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class Scrollbar;


template<>
class Scrollbar<Vertical> : public WndType<Auto, Assigned> {
public:
	Scrollbar(uint width = 20) : width(width) { RegisterChild(slider); }

	// layout
private:
	uint width;
	uint height = 0;
	uint content_height = 0;
	uint slider_height = 0;
	int slider_offset = 0;
private:
	bool IsShown() const { return slider_height < height; }
private:
	virtual void OnSizeRefUpdate(Size size_ref) override { height = size_ref.height; }
	virtual Size GetSize() override { return Size(IsShown() ? width : 0, height); }
public:
	void UpdateScrollOffset(uint content_height, uint frame_height, int frame_offset) {
		bool is_shown = IsShown(); this->content_height = content_height;
		slider_height = (uint)roundf((float)height * frame_height / content_height);
		slider_offset = (int)roundf((float)height * frame_offset / content_height);
		if (is_shown ^ IsShown()) { SizeUpdated(); }
		Redraw();
	}
private:
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if (Interval(slider_offset, slider_height).Contains(point.y)) { return &slider; }
		return this;
	}
private:
	virtual void OnFrameOffsetChange(int scroll_offset) {}

	// paint
private:
	static constexpr Color frame_color = Color::DarkGray;
	static constexpr Color slider_color_normal = Color::Gray;
	static constexpr Color slider_color_hover = Color::DimGray;
	static constexpr Color slider_color_press = 0x606060;
private:
	Color slider_color = slider_color_normal;
private:
	void SetSliderColor(Color color) { slider_color = color; Redraw(); }
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		if (IsShown()) {
			figure_queue.add(point_zero, new Rectangle(GetSize(), frame_color));
			figure_queue.add(Point(0, slider_offset), new Rectangle(Size(width, slider_height), slider_color));
		}
	}

	// message
private:
	int mouse_down_offset = 0;
private:
	void OnMousePress(int y) { mouse_down_offset = y - slider_offset; }
	void OnMouseDrag(int y) { OnFrameOffsetChange((y - mouse_down_offset) * (int)content_height / (int)height); }

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
			}
		}
		virtual void OnNotifyMsg(NotifyMsg msg) override {
			switch (msg) {
			case NotifyMsg::MouseEnter: SetState(State::Hover); break;
			case NotifyMsg::MouseHover: SetCursor(CursorStyle::Default); break;
			case NotifyMsg::MouseLeave: SetState(State::Normal); break;
			}
		}
	}slider;
};


END_NAMESPACE(WndDesign)