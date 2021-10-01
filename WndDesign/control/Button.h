#pragma once

#include "Placeholder.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class Button : public Placeholder<WidthType, HeightType> {
public:
	using Placeholder<WidthType, HeightType>::Placeholder;
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
protected:
	virtual void OnHover() {}
	virtual void OnPress() {}
	virtual void OnClick() {}
	virtual void OnLeave() {}
};


END_NAMESPACE(WndDesign)