#pragma once

#include "Placeholder.h"
#include "../wrapper/Background.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class Button : public Decorate<Placeholder<WidthType, HeightType>, SolidColorBackground> {
public:
	using Decorate<Placeholder<WidthType, HeightType>, SolidColorBackground>::Decorate;

	// style
protected:
	Color background_normal = this->background = Color::DarkGray;
	Color background_hover = Color::Gray;
	Color background_press = Color::DimGray;

	// paint
private:
	void SetBackgroundColor(Color color) { if (this->background != color) { this->background = color; this->Redraw(region_infinite); } }

	// message
private:
	enum class State { Normal, Hover, Press } state = State::Normal;
private:
	void OnHover() { SetBackgroundColor(background_hover); }
	void OnPress() { SetBackgroundColor(background_press); }
	void OnLeave() { SetBackgroundColor(background_normal); }
protected:
	virtual void OnClick() {}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (msg.type) {
		case MouseMsg::LeftDown: state = State::Press; OnPress(); break;
		case MouseMsg::LeftUp: if (state == State::Press) { state = State::Hover; OnHover(); OnClick(); } break;
		}
	}
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		switch (msg) {
		case NotifyMsg::MouseEnter: if (state == State::Normal) { state = State::Hover; OnHover(); } break;
		case NotifyMsg::MouseLeave: state = State::Normal; OnLeave(); break;
		}
	}
};


END_NAMESPACE(WndDesign)