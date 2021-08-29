#pragma once

#include "../figure/color.h"


struct ID2D1Factory1;
struct ID2D1DeviceContext;
struct ID2D1SolidColorBrush;


BEGIN_NAMESPACE(WndDesign)


ID2D1Factory1& GetD2DFactory();
ID2D1DeviceContext& GetD2DDeviceContext();
ID2D1SolidColorBrush& GetD2DSolidColorBrush(Color color);

void BeginDraw();
void EndDraw();


END_NAMESPACE(WndDesign)