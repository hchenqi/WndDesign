#pragma once

#include "ScaleFrame.h"
#include "BorderFrame.h"
#include "../style/length_style.h"
#include "../figure/desktop_layer.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class DesktopFrame : protected ScaleFrame<Assigned, Assigned> {
private:
	friend class Desktop;
	friend struct DesktopFrameApi;

public:
	struct Style {
		LengthStyle width;
		LengthStyle height;
		PositionStyle position;
		Border border;
		std::wstring title;

		Style() : Style(LengthStyle(), LengthStyle(), PositionStyle(), Border(), L"") {}
		Style(LengthStyle width, LengthStyle height, PositionStyle position, Border border, std::wstring title) :
			width(width), height(height), position(position), border(border), title(title) {
		}
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

	// border
protected:
	class BorderFrame : public WndDesign::BorderFrame<Assigned, Assigned> {
	public:
		BorderFrame(Border border, child_ptr child) : WndDesign::BorderFrame<Assigned, Assigned>(border, std::move(child)), border_copy(border) {}
	protected:
		Border border_copy;
	public:
		void Hide() { border = {}; }
		void Restore() { border = border_copy; }
	protected:
		virtual void OnMouseMsg(MouseMsg msg) override;
	};
protected:
	BorderFrame& GetBorder() { return static_cast<BorderFrame&>(*child); }

	// layout
private:
	Rect region;
private:
	Rect GetRegion() { return region; }
	void SetScale(float value) { scale.x = scale.y = value; }
	void SetSize(Size size);
	void SetPoint(Point point) { region.point = point; }

	// hwnd
protected:
	using HANDLE = void*;
	HANDLE hwnd = nullptr;
protected:
	enum class Status { Normal, Minimized, Maximized } status = Status::Normal;
private:
	void SetStatus(Status status);
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
	void Draw();
};


END_NAMESPACE(WndDesign)