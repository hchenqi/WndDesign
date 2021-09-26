#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class Button : public WndObject {
private:
	enum class State { Normal, Hover, Press } state = State::Normal;
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (msg.type) {
		case MouseMsg::LeftDown: state = State::Press; OnPress(); break;
		case MouseMsg::LeftUp: if (state == State::Press) { OnClick(); state = State::Hover; } break;
		}
	}
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		switch (msg) {
		case NotifyMsg::MouseEnter: if (state == State::Normal) { state = State::Hover; OnHover(); } break;
		case NotifyMsg::MouseLeave: state = State::Normal; OnLeave(); break;
		}
	}
protected:
	void OnHover() {}
	void OnPress() {}
	void OnClick() {}
	void OnLeave() {}
};


END_NAMESPACE(WndDesign)