#pragma once

#include "../figure/color.h"


struct ID2D1Factory8;
struct ID2D1DeviceContext7;
struct ID2D1SolidColorBrush;


BEGIN_NAMESPACE(WndDesign)


ID2D1Factory8& GetD2DFactory();
ID2D1DeviceContext7& GetD2DDeviceContext();
ID2D1SolidColorBrush& GetD2DSolidColorBrush(Color color);

void BeginDraw();
void EndDraw();


END_NAMESPACE(WndDesign)