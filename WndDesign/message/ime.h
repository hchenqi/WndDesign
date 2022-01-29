#pragma once

#include "../common/uncopyable.h"
#include "../geometry/point.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

class WndObject;


class ImeApi : public Uncopyable {
private:
	friend class Desktop;
protected:
	ImeApi(ref_ptr<WndObject> wnd) : wnd(*wnd) { if (wnd == nullptr) { throw std::invalid_argument("invalid window pointer"); } ImeEnable(); }
	~ImeApi() { ImeDisable(); }
protected:
	WndObject& wnd;
protected:
	void ImeEnable();
	void ImeDisable();
	void ImeSetPosition(Point point);
protected:
	virtual void OnImeCompositionBegin() {}
	virtual void OnImeComposition(std::wstring str) {}
	virtual void OnImeCompositionEnd() {}
};


END_NAMESPACE(WndDesign)