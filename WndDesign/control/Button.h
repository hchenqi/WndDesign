#pragma once

#include "Placeholder.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class Button : public Placeholder<WidthType, HeightType> {
public:
	using Placeholder<WidthType, HeightType>::Placeholder;
protected:
	Color background_normal = Color::DarkGray;
	Color background_hover = Color::Gray;
	Color background_press = Color::DimGray;
protected:
	Color background = background_normal;
protected:
	void SetBackgroundColor(Color color) { background = color; this->Redraw(region_infinite); }
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(draw_region.point, new Rectangle(draw_region.size, background));
	}
private:
	void OnHover() { SetBackgroundColor(background_hover); }
	void OnPress() { SetBackgroundColor(background_press); }
	void OnLeave() { SetBackgroundColor(background_normal); }
private:
	virtual void OnClick() {}
private:
	enum class State { Normal, Hover, Press } state = State::Normal;
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (msg.type) {
		case MouseMsg::LeftDown: state = State::Press; OnPress(); break;
		case MouseMsg::LeftUp: if (state == State::Press) { OnClick(); state = State::Hover; } break;
		case MouseMsg::WheelVertical: case MouseMsg::WheelHorizontal: return this->PassMouseMsg(msg);
		}
	}
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		switch (msg) {
		case NotifyMsg::MouseEnter: if (state == State::Normal) { state = State::Hover; OnHover(); } break;
		case NotifyMsg::MouseHover: SetCursor(Cursor::Default); break;
		case NotifyMsg::MouseLeave: state = State::Normal; OnLeave(); break;
		}
	}
};


END_NAMESPACE(WndDesign)