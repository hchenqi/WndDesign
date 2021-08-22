#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction, class SizeType>
class TextBox;


template<>
class TextBox<Vertical, Relative> : public WndType<Relative, Auto> {

private:	
	virtual const Size OnSizeRefChange(Size size_ref) {
		return layout.AutoResize(Size(size_ref.width, length_max));
	}
};


template<>
class TextBox<Vertical, Auto> : public WndType<Auto, Auto> {

private:
	virtual const Size OnSizeRefChange(Size size_ref) {
		return layout.AutoResize(Size(length_max, length_max));
	}
};


END_NAMESPACE(WndDesign)