#pragma once

#include "../geometry/point.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)

class WndObject;


class ABSTRACT_BASE ImeMsgHandler {
private:
	friend class Desktop;
	WndObject& wnd;
protected:
	ImeMsgHandler(WndObject& wnd);
protected:
	void ImeEnable();
	void ImeDisable();
	void ImeSetPosition(Point point);
private:
	virtual void OnImeCompositionBegin() pure;
	virtual void OnImeComposition(std::wstring str) pure;
	virtual void OnImeCompositionEnd() pure;
};


END_NAMESPACE(WndDesign)