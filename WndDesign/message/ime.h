#pragma once

#include "../common/uncopyable.h"
#include "../geometry/point.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

class WndObject;


class ImeApi : public Uncopyable {
private:
	friend class Desktop;
	WndObject& wnd;
protected:
	ImeApi(ref_ptr<WndObject> wnd);
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