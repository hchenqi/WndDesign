#pragma once

#include "WndFrame.h"
#include "../wrapper/Border.h"
#include "../style/style.h"
#include "../figure/desktop_layer.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class DesktopFrame : private Decorate<WndFrame, Border> {
private:
	friend class Desktop;
	friend struct DesktopFrameApi;

public:
	using child_ptr = child_ptr<Assigned, Assigned>;

public:
	struct Style {
		LengthStyle width, height;
		PositionStyle position;
		Border::Style border;
		std::wstring title;
	};

public:
	DesktopFrame(Style style, child_ptr child);
	~DesktopFrame();

	// style
private:
	LengthStyle width, height;
private:
	std::pair<Size, Rect> GetMinMaxRegion() const;
protected:
	void SetTitle(std::wstring title);

	// layout
private:
	Point point;
private:
	Rect GetRegion() { return Rect(point, size); }
	void SetSize(Size size);
	void SetPoint(Point point) { this->point = point; }
private:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;

	// hwnd
protected:
	using HANDLE = void*;
	HANDLE hwnd = nullptr;
protected:
	enum class Status { Normal, Minimized, Maximized } status = Status::Normal;
private:
	void SetStatus(Status status) { this->status = status; }
protected:
	void Show();
	void Minimize();
	void Maximize();
	void Restore();
	void Destroy();

	// paint
private:
	DesktopLayer layer;
	Region invalid_region;
private:
	void ResizeLayer();
	void RecreateLayer();
private:
	void Redraw(Rect redraw_region);
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	void Draw();

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override;
};


END_NAMESPACE(WndDesign)