#pragma once

#include "wrapper.h"


BEGIN_NAMESPACE(WndDesign)


template<class Wnd, Cursor cursor = Cursor::Arrow>
class CustomizedCursor : public Wnd {
public:
	using Base = CustomizedCursor;

public:
	template <typename... Args>
	CustomizedCursor(Args&&... args) : Wnd(std::forward<Args>(args)...) {
		this->cursor = cursor;
	}
};


END_NAMESPACE(WndDesign)