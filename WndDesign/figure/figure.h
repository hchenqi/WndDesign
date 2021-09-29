#pragma once

#include "../geometry/geometry.h"


BEGIN_NAMESPACE(WndDesign)

struct RenderTarget;

struct ABSTRACT_BASE Figure {
	virtual void DrawOn(RenderTarget& target, Point point) const pure;
};


const Size GetTargetSize(const RenderTarget& target);


END_NAMESPACE(WndDesign)