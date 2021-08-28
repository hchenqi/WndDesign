#pragma once

#include "../geometry/geometry.h"


BEGIN_NAMESPACE(WndDesign)

class RenderTarget;


struct ABSTRACT_BASE Figure {
	virtual void DrawOn(RenderTarget& target, Point point) const pure;
};


END_NAMESPACE(WndDesign)