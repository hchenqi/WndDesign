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
	Size size;
	Size child_size;
	Point frame_offset;
protected:
	Size GetFrameSize() const { return size; }
	Size GetChildSize() const { return child_size; }
	Point GetFrameOffset() const { return frame_offset; }
	Vector GetChildOffset() const { return point_zero - frame_offset; }
protected:
	virtual Transform GetChildTransform(WndObject& child) const override { return GetChildOffset(); }

	// scrolling
protected:
	Point ClampFrameOffset(Point offset) const {
		offset.x = child_size.width <= size.width ? 0.0f : clamp(offset.x, Interval(0.0f, child_size.width - size.width));
		offset.y = child_size.height <= size.height ? 0.0f : clamp(offset.y, Interval(0.0f, child_size.height - size.height));
		return offset;
	}
public:
	void ScrollTo(Point offset) {
		offset = ClampFrameOffset(offset);
		if (frame_offset != offset) {
			frame_offset = offset;
			Redraw(region_infinite);
		}
	}
	void Scroll(Vector offset) {
		if (offset != vector_zero) {
			ScrollTo(frame_offset + offset);
		}
	}
	void ScrollIntoView(Point point) {
		point = clamp(point, Rect(point_zero, child_size));
		Scroll(point - clamp(point, Rect(frame_offset, size)));
	}
	void ScrollIntoView(Rect rect) {
		rect = clamp(rect, Rect(point_zero, child_size));
		Scroll(rect.point - clamp(rect, Rect(frame_offset, size)).point);
	}

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Redraw(child_redraw_region + GetChildOffset());
	}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		DrawChild(child, point_zero + GetChildOffset(), figure_queue, draw_region);
	}

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override { msg.point -= GetChildOffset(); return WndFrame::HitTest(msg); }
};


template<>
class ScrollFrame<Bidirectional> : public _ScrollFrame_Base {
public:
	ScrollFrame(child_ptr<Auto, Auto> child) : _ScrollFrame_Base(std::move(child)) {
		child_size = UpdateChildSizeRef(this->child, size_empty);
	}

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			size = size_ref;
			frame_offset = ClampFrameOffset(frame_offset);
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (this->child_size != child_size) {
			this->child_size = child_size;
			frame_offset = ClampFrameOffset(frame_offset);
			Redraw(region_infinite);
		}
	}

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (!msg.ctrl && (msg.type == MouseMsg::WheelVertical || msg.type == MouseMsg::WheelHorizontal)) { return this; }
		return _ScrollFrame_Base::HitTest(msg);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (msg.type) {
		case MouseMsg::WheelVertical: Scroll(Vector(0, -(float)msg.wheel_delta)); break;
		case MouseMsg::WheelHorizontal: Scroll(Vector(-(float)msg.wheel_delta, 0)); break;
		}
	}
};


template<>
class ScrollFrame<Vertical> : public _ScrollFrame_Base {
public:
	ScrollFrame(child_ptr<Assigned, Auto> child) : _ScrollFrame_Base(std::move(child)) {}

	// layout
public:
	float GetFrameLength() const { return _ScrollFrame_Base::GetFrameSize().height; }
	float GetChildLength() const { return _ScrollFrame_Base::GetChildSize().height; }
	float GetFrameOffset() const { return _ScrollFrame_Base::GetFrameOffset().y; }

	// scroll
public:
	using _ScrollFrame_Base::ScrollTo;
	using _ScrollFrame_Base::Scroll;
	using _ScrollFrame_Base::ScrollIntoView;
public:
	void ScrollTo(float offset) { ScrollTo(Point(0.0f, offset)); }
	void Scroll(float offset) { Scroll(Vector(0.0f, offset)); }
	void ScrollIntoView(float y) { ScrollIntoView(Point(0.0f, y)); }
	void ScrollIntoView(Interval interval) { ScrollIntoView(Rect(Interval(0.0f, size.width), interval)); }

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			if (size.width != size_ref.width) {
				child_size = Size(size_ref.width, UpdateChildSizeRef(child, Size(size_ref.width, length_max)).height);
			}
			size = size_ref;
			frame_offset = ClampFrameOffset(frame_offset);
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (this->child_size.height != child_size.height) {
			this->child_size.height = child_size.height;
			frame_offset = ClampFrameOffset(frame_offset);
			Redraw(region_infinite);
		}
	}

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (!msg.ctrl && msg.type == MouseMsg::WheelVertical) { return this; }
		return _ScrollFrame_Base::HitTest(msg);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		Scroll(-(float)msg.wheel_delta);
	}
};


template<>
class ScrollFrame<Horizontal> : public _ScrollFrame_Base {
public:
	ScrollFrame(child_ptr<Auto, Assigned> child) : _ScrollFrame_Base(std::move(child)) {}

	// layout
public:
	float GetFrameLength() const { return _ScrollFrame_Base::GetFrameSize().width; }
	float GetChildLength() const { return _ScrollFrame_Base::GetChildSize().width; }
	float GetFrameOffset() const { return _ScrollFrame_Base::GetFrameOffset().x; }

	// scroll
public:
	using _ScrollFrame_Base::ScrollTo;
	using _ScrollFrame_Base::Scroll;
	using _ScrollFrame_Base::ScrollIntoView;
public:
	void ScrollTo(float offset) { ScrollTo(Point(offset, 0.0f)); }
	void Scroll(float offset) { Scroll(Vector(offset, 0.0f)); }
	void ScrollIntoView(float x) { ScrollIntoView(Point(x, 0.0f)); }
	void ScrollIntoView(Interval interval) { ScrollIntoView(Rect(interval, Interval(0.0f, size.height))); }

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		if (size != size_ref) {
			if (size.height != size_ref.height) {
				child_size = Size(UpdateChildSizeRef(child, Size(length_max, size_ref.height)).width, size_ref.height);
			}
			size = size_ref;
			frame_offset = ClampFrameOffset(frame_offset);
		}
		return size;
	}
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override {
		if (this->child_size.width != child_size.width) {
			this->child_size.width = child_size.width;
			frame_offset = ClampFrameOffset(frame_offset);
			Redraw(region_infinite);
		}
	}

	// message
protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (!msg.ctrl && msg.type == MouseMsg::WheelHorizontal) { return this; }
		return _ScrollFrame_Base::HitTest(msg);
	}
protected:
	virtual void OnMouseMsg(MouseMsg msg) override {
		Scroll(-(float)msg.wheel_delta);
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
ScrollFrame(T) -> ScrollFrame<typename deduce_scroll_frame_direction<extract_width_type<T>, extract_height_type<T>>::type>;


END_NAMESPACE(WndDesign)