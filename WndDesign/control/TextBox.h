#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class TextBoxLeftRightTopBottom : public WndObject {



private:	
	virtual const Size OnSizeRefChange(Size size_ref) {
		return layout.AutoResize(Size(size_ref.width, length_max));
	}
};


END_NAMESPACE(WndDesign)