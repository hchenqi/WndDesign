#pragma once

#include "../style/border_helper.h"


BEGIN_NAMESPACE(WndDesign)

class WndObject;


void AeroSnapDraggingEffect(WndObject& wnd, Point point);

void AeroSnapBorderResizingEffect(WndObject& wnd, Point point, BorderPosition border_position);


END_NAMESPACE(WndDesign)