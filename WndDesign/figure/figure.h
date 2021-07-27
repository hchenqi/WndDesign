#pragma once

#include "../geometry/geometry.h"


BEGIN_NAMESPACE(WndDesign)

class RenderTarget;


struct ABSTRACT_BASE Figure {
	virtual const Rect GetRegion() const pure;
	virtual void DrawOn(RenderTarget& target, Point point) const pure;
};


END_NAMESPACE(WndDesign)