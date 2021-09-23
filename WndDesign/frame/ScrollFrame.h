#pragma once

#include "WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ScrollFrame;


template<>
class ScrollFrame<Vertical> : public WndFrame, public LayoutType<Relative, Relative> {
public:
	using child_ptr = WndDesign::child_ptr<Relative, Auto>;
public:
	ScrollFrame(child_ptr child) : WndFrame(std::move(child)) {}
private:
	uint child_height = 0;
	uint frame_height = 0;
	int frame_offset = 0;
private:
	static int Clamp(int position, int position_min, int position_max) {
		if (position < position_min) { position = position_min; }
		if (position > position_max) { position = position_max; }
		return position;
	}
	static int Clamp(int position, uint length, int position_min, int position_max) {
		return Clamp(Clamp(position + (int)length, position_min, position_max) - (int)length, position_min, position_max);
	}
private:
	void UpdateFrameOffset(int frame_offset) {
		frame_offset = child_height < frame_height ? 0 : Clamp(frame_offset, 0, (int)(child_height - frame_height));
		if (this->frame_offset != frame_offset) { this->frame_offset = frame_offset; Redraw(region_infinite); }
	}
public:
	int Scroll(int offset) {
		int assumed_frame_offset = frame_offset + offset;
		UpdateFrameOffset(assumed_frame_offset);
		return assumed_frame_offset - frame_offset;
	}
	void ScrollIntoView(int y) {
		Scroll(y - Clamp(y, frame_offset, frame_offset + (int)frame_height));
	}
	void ScrollIntoView(int y, uint length) {
		Scroll(y - Clamp(y, length, frame_offset, frame_offset + (int)frame_height));
	}
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		child_height = UpdateChildSizeRef(child, Size(size_ref.width, length_max)).height;
		frame_height = size_ref.height; UpdateFrameOffset(frame_offset); return Size();
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		child_height = child_size.height; UpdateFrameOffset(frame_offset);
	}
private:
	virtual const Vector GetChildOffset(WndObject& child) const { 
		return Vector(0, -frame_offset); 
	}
	virtual void OnChildScrollIntoView(WndObject& child, Rect region) {
		ScrollIntoView(region.point.y, region.size.height);
	}
	virtual ref_ptr<WndObject> HitTest(Point& point) override {
		if ((uint)(point.y += frame_offset) < child_height) { return child; }
		return nullptr;
	}
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override {
		redraw_region.point.y -= frame_offset; Redraw(redraw_region);
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override {
		DrawChild(child, Point(0, -frame_offset), figure_queue, draw_region);
	}
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { Scroll(-msg.wheel_delta); }
	}
};


END_NAMESPACE(WndDesign)