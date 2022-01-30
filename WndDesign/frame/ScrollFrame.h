#pragma once

#include "WndFrame.h"
#include "../geometry/clamp.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ScrollFrame;


class _ScrollFrame_Base : public WndFrame, public LayoutType<Assigned, Assigned> {
protected:
	_ScrollFrame_Base(child_ptr<> child) : WndFrame(std::move(child)) {}

	// layout
protected:
	Size child_size;
	Point frame_offset;
public:
	Size GetFrameSize() const { return size; }
	Size GetChildSize() const { return child_size; }
	Point GetFrameOffset() const { return frame_offset; }
protected:
	Vector GetChildOffset() const { return point_zero - frame_offset; }

	// scroll
public:
	void UpdateFrameOffset(Point offset) {
		frame_offset.x = child_size.width <= size.width ? 0.0f : clamp(offset.x, Interval(0.0f, child_size.width - size.width));
		frame_offset.y = child_size.height <= size.height ? 0.0f : clamp(offset.y, Interval(0.0f, child_size.height - size.height));
		OnFrameOffsetUpdate();
		redraw_region = region_infinite; Redraw();
	}
	void Scroll(Vector offset) {
		if (offset == vector_zero) { return; }
		UpdateFrameOffset(frame_offset + offset);
	}
	void ScrollIntoView(Point point) {
		Scroll(point - clamp(point, Rect(frame_offset, size)));
	}
	void ScrollIntoView(Rect rect) {
		Scroll(rect.point - clamp(rect, Rect(frame_offset, size)));
	}
protected:
	virtual void OnFrameOffsetUpdate() {}

	// layout
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			size = size_ref;
			child_size = UpdateChildSizeRef(child, Size(length_max, length_max));
			UpdateFrameOffset(frame_offset);
		}
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (this->child_size != child_size) {
			this->child_size = child_size;
			UpdateFrameOffset(frame_offset);
		}
	}
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override { point -= GetChildOffset(); return child; }
	virtual Transform GetChildTransform(WndObject& child) override { return GetChildOffset(); }

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		redraw_region = child_redraw_region + GetChildOffset(); Redraw();
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}

	// message
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { Scroll(Vector((float)-msg.wheel_delta, 0.0f)); }
		if (msg.type == MouseMsg::WheelHorizontal) { Scroll(Vector(0.0f, (float)-msg.wheel_delta)); }
	}
};


template<>
class ScrollFrame<Bidirectional> : public _ScrollFrame_Base {
public:
	using child_ptr = child_ptr<Auto, Auto>;

public:
	ScrollFrame(child_ptr child) : _ScrollFrame_Base(std::move(child)) {}
};


template<>
class ScrollFrame<Vertical> : public _ScrollFrame_Base {
public:
	using child_ptr = child_ptr<Assigned, Auto>;

public:
	ScrollFrame(child_ptr child) : _ScrollFrame_Base(std::move(child)) {}

	// layout
public:
	float GetFrameLength() const { return _ScrollFrame_Base::GetFrameSize().height; }
	float GetChildLength() const { return _ScrollFrame_Base::GetChildSize().height; }
	float GetFrameOffset() const { return _ScrollFrame_Base::GetFrameOffset().y; }

	// scroll
public:
	void UpdateFrameOffset(float offset) { _ScrollFrame_Base::UpdateFrameOffset(Point(0.0f, offset)); }
	void Scroll(float offset) { _ScrollFrame_Base::Scroll(Vector(0.0f, offset)); }
	void ScrollIntoView(float y) { _ScrollFrame_Base::ScrollIntoView(Point(0.0f, y)); }
	void ScrollIntoView(float begin, float length) { _ScrollFrame_Base::ScrollIntoView(Rect(0.0f, begin, 1.0f, length)); }

	// layout
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			if (size.width != size_ref.width) {
				child_size.height = UpdateChildSizeRef(child, Size(size_ref.width, length_max)).height;
			}
			size = size_ref;
			UpdateFrameOffset(frame_offset.y);
		}
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (this->child_size.height != child_size.height) {
			this->child_size.height = child_size.height;
			UpdateFrameOffset(frame_offset.y);
		}
	}

	// message
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelVertical) { Scroll((float)-msg.wheel_delta); }
	}
};


template<>
class ScrollFrame<Horizontal> : public _ScrollFrame_Base {
public:
	using child_ptr = child_ptr<Auto, Assigned>;

public:
	ScrollFrame(child_ptr child) : _ScrollFrame_Base(std::move(child)) {}

	// layout
public:
	float GetFrameLength() const { return _ScrollFrame_Base::GetFrameSize().width; }
	float GetChildLength() const { return _ScrollFrame_Base::GetChildSize().width; }
	float GetFrameOffset() const { return _ScrollFrame_Base::GetFrameOffset().x; }

	// scroll
public:
	void UpdateFrameOffset(float offset) { _ScrollFrame_Base::UpdateFrameOffset(Point(offset, 0.0f)); }
	void Scroll(float offset) { _ScrollFrame_Base::Scroll(Vector(offset, 0.0f)); }
	void ScrollIntoView(float x) { _ScrollFrame_Base::ScrollIntoView(Point(x, 0.0f)); }
	void ScrollIntoView(float begin, float length) { _ScrollFrame_Base::ScrollIntoView(Rect(begin, 0.0f, length, 1.0f)); }

	// layout
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			if (size.height != size_ref.height) {
				child_size.width = UpdateChildSizeRef(child, Size(length_max, size_ref.height)).width;
			}
			size = size_ref;
			UpdateFrameOffset(frame_offset.x);
		}
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (this->child_size.width != child_size.width) {
			this->child_size.width = child_size.width;
			UpdateFrameOffset(frame_offset.x);
		}
	}

	// message
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::WheelHorizontal) { Scroll((float)-msg.wheel_delta); }
	}
};


template<class WidthType, class HeightType>
struct deduce_scroll_frame_direction;

template<>
struct deduce_scroll_frame_direction<Auto, Auto> {
	using type = Bidirectional;
};

template<>
struct deduce_scroll_frame_direction<Assigned, Auto> {
	using type = Vertical;
};

template<>
struct deduce_scroll_frame_direction<Auto, Assigned> {
	using type = Horizontal;
};


template<class T>
ScrollFrame(T)->ScrollFrame<typename deduce_scroll_frame_direction<extract_width_type<T>, extract_height_type<T>>::type>;


END_NAMESPACE(WndDesign)