#pragma once

#include "../window/wnd_traits.h"

#include <memory>
#include <string>


BEGIN_NAMESPACE(WndDesign)

class DesktopWndLayer;


class DesktopWndFrame : private WndObject {
private:
	friend class Desktop;
	using child_ptr = child_ptr<>;
private:
	DesktopWndFrame(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	child_ptr child;
protected:
	using HANDLE = void*;
	HANDLE hwnd;
private:
	std::unique_ptr<DesktopWndLayer> layer;
	Rect region;
public:
	void UpdateInvalidRegion();
	void Present();
	void RecreateTarget();

public:
	const std::pair<> GetMinMaxRegion() const {}
	const Rect GetRegion() const {}

protected:
	void SetTitle(const wstring& title) {}
	void Show() {}
	void Minimize() {}
	void Maximize() {}

private:
	virtual void OnCreate() {

	}

private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		size_ref = CalculateSize(size_ref);
		if (size != size_ref) {
			size = size_ref;
			layer->
		}
		return UpdateChildSizeRef(child, size_ref); 
	}
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override {

	}
	void Draw(Rect draw_region) const;
public:
	virtual void OnMouseMsg(MouseMsg& msg) override { SendChildMouseMsg(child, msg); }
	virtual void OnKeyMsg(KeyMsg msg) override {}
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		if (msg == NotifyMsg::MouseLeave && )
	}
};


END_NAMESPACE(WndDesign)