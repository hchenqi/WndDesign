#pragma once

#include "../window/WndObject.h"


BEGIN_NAMESPACE(WndDesign)


class WndFrame : public WndObject {


private:
	WndObject& child;


private:
	virtual const Size OnSizeRefChange(Size size_ref) {
	

	}

	virtual void OnChildSizeChange(WndObject& child, Size child_size) {
	
	}




};


END_NAMESPACE(WndDesign)