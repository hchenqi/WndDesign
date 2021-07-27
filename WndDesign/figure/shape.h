#pragma once

#include "figure.h"
#include "color.h"


BEGIN_NAMESPACE(WndDesign)


struct Line : Figure {
	Vector end;
	Color color;
	float width;

	Line(Vector end, Color color, float width) :
		end(end), color(color), width(width) {
	}

	virtual const Rect GetRegion() const override {
		int x, y; uint w, h;
		if (end.x < 0) { x = end.x; w = (uint)-end.x; } else { x = 0; w = (uint)end.x; }
		if (end.y < 0) { y = end.y; h = (uint)-end.y; } else { y = 0; h = (uint)end.y; }
		return Rect(x, y, w, h);
	}
	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


struct Rectangle : Figure {
	Size size;
	Color fill_color;
	float border_width;
	Color border_color;

	Rectangle(Size size, Color fill_color, float border_width, Color border_color) :
		size(size), fill_color(fill_color), border_width(border_width), border_color(border_color) {
	}
	Rectangle(Size size, Color fill_color) :
		Rectangle(size, fill_color, 0, color_transparent) {
	}
	Rectangle(Size size, float border_width, Color border_color) :
		Rectangle(size, color_transparent, border_width, border_color) {
	}

	virtual const Rect GetRegion() const override { return Rect(point_zero, size); }
	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


struct RoundedRectangle : Figure {
	Size size;
	uint radius;
	Color fill_color;
	float border_width;
	Color border_color;

	RoundedRectangle(Size size, uint radius, Color fill_color, float border_width, Color border_color) :
		size(size), radius(radius), fill_color(fill_color), border_width(border_width), border_color(border_color) {
	}
	RoundedRectangle(Size size, uint radius, Color fill_color) :
		RoundedRectangle(size, radius, fill_color, 0, color_transparent) {
	}
	RoundedRectangle(Size size, uint radius, float border_width, Color border_color) :
		RoundedRectangle(size, radius, color_transparent, border_width, border_color) {
	}

	virtual const Rect GetRegion() const override { return Rect(point_zero, size); }
	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


struct Ellipse : Figure {
	uint radius_x;
	uint radius_y;
	Color fill_color;
	float border_width;
	Color border_color;

	Ellipse(uint radius_x, uint radius_y, Color fill_color, float border_width, Color border_color) :
		radius_x(radius_x), radius_y(radius_y), fill_color(fill_color), border_width(border_width), border_color(border_color) {
	}
	Ellipse(uint radius_x, uint radius_y, Color fill_color) :
		Ellipse(radius_x, radius_y, fill_color, 0, color_transparent) {
	}
	Ellipse(uint radius_x, uint radius_y, float border_width, Color border_color) :
		Ellipse(radius_x, radius_y, color_transparent, border_width, border_color) {
	}

	virtual const Rect GetRegion() const override { return Rect(-(int)radius_x, -(int)radius_y, 2 * radius_x, 2 * radius_y); }
	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


struct Circle : public Ellipse {
	Circle(uint radius, Color fill_color, float border_width, Color border_color) :
		Ellipse(radius, radius, fill_color, border_width, border_color) {
	}
	Circle(uint radius, Color fill_color) :
		Ellipse(radius, radius, fill_color, 0, color_transparent) {
	}
	Circle(uint radius, float border_width, Color border_color) :
		Ellipse(radius, radius, color_transparent, border_width, border_color) {
	}
};


END_NAMESPACE(WndDesign)