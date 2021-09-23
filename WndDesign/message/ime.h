#pragma once

#include "../common/uncopyable.h"
#include "../geometry/point.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

class WndObject;


class ImeMsgHandler : public Uncopyable {
private:
	friend class Desktop;
	WndObject& wnd;
protected:
	ImeMsgHandler(WndObject& wnd);
	ImeMsgHandler(ref_ptr<WndObject> wnd) : ImeMsgHandler([=]() -> WndObject& {
		if (wnd == nullptr) { throw std::invalid_argument("invalid window pointer"); } return *wnd; }()) {
	}
protected:
	void ImeEnable();
	void ImeDisable();
	void ImeSetPosition(Point point);
private:
	virtual void OnImeCompositionBegin() {}
	virtual void OnImeComposition(std::wstring str) {}
	virtual void OnImeCompositionEnd() {}
};


END_NAMESPACE(WndDesign)