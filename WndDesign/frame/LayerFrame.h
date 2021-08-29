#pragma once

#include "../window/wnd_traits.h"
#include "../figure/layer.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class LayerFrame : public WndType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	LayerFrame(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	child_ptr child;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { 
		return UpdateChildSizeRef(child, size_ref); 
	}
	virtual void OnChildSizeUpdate(const WndObject& child, Size child_size) override {

		// Update Layer Size

		SizeUpdated(child_size);
	}
private:
	virtual void OnChildRedraw(const WndObject& child, Rect redraw_region) {
		
	}
	virtual void OnPaint() override {
		// paint layer
	}
	virtual void OnChildFigureUpdate() override {
		// paint child on layer
		// FigureUpdated()
	}
};


template<class WidthType, class HeightType>
class TiledLayerFrame : public WndType<WidthType, HeightType> {
public:
	using child_ptr = child_ptr<WidthType, HeightType>;
public:
	LayerFrame(child_ptr child) : child(std::move(child)) { RegisterChild(this->child); }
private:
	child_ptr child;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		return UpdateChildSizeRef(child, size_ref);
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {

		// Update Layer Size

		SizeUpdated(child_size);
	}
};


END_NAMESPACE(WndDesign)