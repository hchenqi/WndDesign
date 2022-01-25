#pragma once

#include "wrapper.h"
#include "../system/cursor.h"


BEGIN_NAMESPACE(WndDesign)


template<class Wnd>
class Cursor : public Wnd {
public:
	using Wnd::Wnd;

	// style
protected:
	struct Style {
	public:
		CursorStyle _cursor = CursorStyle::Default;
	public:
		void set(CursorStyle cursor) { _cursor = cursor; }
	}cursor;

	// message
protected:
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		switch (msg) {
		case NotifyMsg::MouseHover: SetCursor(cursor._cursor); break;
		}
		Wnd::OnNotifyMsg(msg);
	}
};


END_NAMESPACE(WndDesign)