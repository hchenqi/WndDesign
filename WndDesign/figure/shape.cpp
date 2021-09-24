#include "shape.h"

#include "../system/d2d_api.h"
#include "../system/directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


inline D2D1_RECT_F ShrinkD2DRectByLength(D2D1_RECT_F rect, float length) {
	return D2D1::RectF(rect.left + length, rect.top + length, rect.right - length, rect.bottom - length);
}


void Line::DrawOn(RenderTarget& target, Point point) const {
	if (width > 0 && color.IsVisible()) {
		target.DrawLine(Point2POINT(point), Point2POINT(point + end), &GetD2DSolidColorBrush(color), width);
	}
}


void Rectangle::DrawOn(RenderTarget& target, Point point) const {
	if (fill_color.IsVisible()) {
		target.FillRectangle(Rect2RECT(Rect(point, size)), &GetD2DSolidColorBrush(fill_color));
	}
	if (border_width > 0 && border_color.IsVisible()) {
		target.DrawRectangle(ShrinkD2DRectByLength(Rect2RECT(Rect(point, size)), border_width / 2), &GetD2DSolidColorBrush(border_color), border_width);
	}
}


void RoundedRectangle::DrawOn(RenderTarget& target, Point point) const {
	if (fill_color.IsVisible()) {
		target.FillRoundedRectangle(D2D1::RoundedRect(Rect2RECT(Rect(point, size)), (FLOAT)radius, (FLOAT)radius), &GetD2DSolidColorBrush(fill_color));
	}
	if (border_width > 0 && border_color.IsVisible()) {
		target.DrawRoundedRectangle(D2D1::RoundedRect(ShrinkD2DRectByLength(Rect2RECT(Rect(point, size)), border_width / 2), (FLOAT)radius, (FLOAT)radius), &GetD2DSolidColorBrush(border_color), border_width);
	}
}


void Ellipse::DrawOn(RenderTarget& target, Point point) const {
	if (fill_color.IsVisible()) {
		target.FillEllipse(D2D1::Ellipse(Point2POINT(point), (FLOAT)radius_x, (FLOAT)radius_y), &GetD2DSolidColorBrush(fill_color));
	}
	if (border_width > 0 && border_color.IsVisible()) {
		target.DrawEllipse(D2D1::Ellipse(Point2POINT(point), (FLOAT)radius_x - border_width / 2, (FLOAT)radius_y - border_width / 2), &GetD2DSolidColorBrush(border_color), border_width);
	}
}


END_NAMESPACE(WndDesign)