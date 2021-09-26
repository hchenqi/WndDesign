#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ScrollFrame;


template<>
class ScrollFrame<Vertical> : public WndType<Assigned, Assigned> {
public:
	using child_ptr = child_ptr<Assigned, Auto>;
public:
	ScrollFrame(child_ptr child) : child(std::move(child)) { RegisterChild(*this->child); }
protected:
	child_ptr child;
private:
	uint child_height = 0;
	uint frame_height = 0;
	int frame_offset = 0;
private:
	Vector GetChildOffset() const { return Vector(0, -frame_offset); }
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
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		child_height = UpdateChildSizeRef(child, Size(size_ref.width, length_max)).height;
		frame_height = size_ref.height; UpdateFrameOffset(frame_offset); return size_ref;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		child_height = child_size.height; UpdateFrameOffset(frame_offset);
	}
private:
	virtual Vector GetChildOffset(WndObject& child) override { return GetChildOffset(); }
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
private:
	virtual void OnChildRedraw(WndObject& child, Rect redraw_region) override { Redraw(redraw_region + GetChildOffset()); }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { Scroll(-msg.wheel_delta); }
	}
};


END_NAMESPACE(WndDesign)