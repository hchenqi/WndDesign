#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class ClipFrame : public WndFrame, public LayoutType<Assigned, Assigned> {
public:
	ClipFrame(child_ptr<Assigned, Auto> child) : WndFrame(std::move(child)) {}
	ClipFrame(child_ptr<Auto, Assigned> child) : WndFrame(std::move(child)) {}
	ClipFrame(child_ptr<Auto, Auto> child) : WndFrame(std::move(child)) {}
private:
	virtual Size OnSizeRefUpdate(Size size_ref) override { UpdateChildSizeRef(child, size_ref); return size_ref; }
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {}
};


END_NAMESPACE(WndDesign)