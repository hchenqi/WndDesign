#pragma once

#include "WndFrame.h"
#include "../figure/desktop_layer.h"
#include "../geometry/scale.h"
#include "../geometry/region.h"


BEGIN_NAMESPACE(WndDesign)


class DesktopFrame : protected WndFrame {
private:
	friend class Desktop;
	friend struct DesktopFrameApi;

public:
	DesktopFrame(std::wstring title, child_ptr<> child);
	~DesktopFrame();

	// style
protected:
	void SetTitle(std::wstring title);

	// layout
private:
	Scale scale;
	Rect region;
private:
	Rect GetRegion() { return region; }
	void SetScale(float value) { scale = Scale(value); }
	void SetSize(Size size);
	void SetPoint(Point point) { region.point = point; }
	std::pair<Size, Rect> GetMinMaxRegion(Size size_ref);
	void InitializeRegion(Size size_ref);
protected:
	void DesktopFrameRegionUpdated(Rect region);
protected:
	virtual std::pair<Size, Size> CalculateMinMaxSize(Size size_ref) { return { size_empty, size_ref }; }
	virtual Rect OnDesktopFrameSizeRefUpdate(Size size_ref) { UpdateChildSizeRef(child, size_ref); return Rect(point_zero, size_ref); }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}
private:
	virtual ref_ptr<WndObject> HitTest(Point& point) override final { point = point * scale.Invert(); return child; }
	virtual Transform GetChildTransform(WndObject& child) const override final { return scale; }
private:
	void SizeUpdated() {}  // never used
	virtual Size OnSizeRefUpdate(Size size_ref) override final { return size_ref; }  // never used

	// state
protected:
	enum class State { Normal, Minimized, Maximized };
private:
	State state = State::Normal;
private:
	void SetState(State state) { this->state = state; OnStateChange(state); }
protected:
	State GetState() { return state; }
protected:
	virtual void OnStateChange(State state) {}
protected:
	void Show();
	void Hide();
	void Minimize();
	void Maximize();
	void Restore();
	void Destroy();

	// paint
public:
	using HANDLE = void*;
private:
	HANDLE hwnd = nullptr;
	DesktopLayer layer;
	Region invalid_region;
public:
	HANDLE GetHWND() const { return hwnd; }
private:
	void ResizeLayer();
	void RecreateLayer();
protected:
	void Redraw(Rect redraw_region);
	WndFrame::OnDraw;
private:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override final { Redraw(child_redraw_region); }
	void OnDraw();
};


END_NAMESPACE(WndDesign)