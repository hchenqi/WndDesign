#pragma once

#include "../style/border_helper.h"


BEGIN_NAMESPACE(WndDesign)

class WndObject;


void AeroSnapDraggingEffect(WndObject& wnd);

void AeroSnapBorderResizingEffect(WndObject& wnd, BorderPosition border_position);


END_NAMESPACE(WndDesign)