#pragma once

#include "BorderFrame.h"
#include "../style/style.h"
#include "../figure/desktop_layer.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class DesktopFrame : protected BorderFrame<Assigned, Assigned> {
private:
	friend class Desktop;
	friend struct DesktopFrameApi;

public:
	using child_ptr = BorderFrame::child_ptr;

public:
	struct Style {
		LengthStyle width, height;
		PositionStyle position;
		BorderStyle border;
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
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override;
	void Draw();

	// message
private:
	virtual void OnMouseMsg(MouseMsg msg) override;
};


END_NAMESPACE(WndDesign)