#pragma once

#include "../window/WndObject.h"
#include "../style/value_tag.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {
public:
	struct Style {
		struct BorderStyle {
		public:
			struct BorderLineStyle {
			public:
				uint _width;
				Color _color;
			public:
				constexpr BorderLineStyle& width(ValuePixel width) { _width = width.AsUnsigned(); return *this; }
				constexpr BorderLineStyle& color(Color color) { _color = color; return *this; }
			} left, top, right, bottom;
			uint _radius;
		public:
			constexpr BorderStyle& radius(uint radius) { _radius = radius; return *this; }
		} border;

		struct PaddingStyle {

		} padding;
	};


private:
	WndObject& child;


private:
	virtual const Size OnSizeRefChange(Size size_ref) {
	

	}

	virtual void OnChildSizeChange(WndObject& child, Size child_size) {
	
	}




};


END_NAMESPACE(WndDesign)