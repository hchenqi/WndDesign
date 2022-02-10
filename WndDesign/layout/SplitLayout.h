#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


struct First {};
struct Second {};


template<class Direction, class AutoChild>
class SplitLayout;


template<>
class SplitLayout<Vertical, First> : public WndType<Assigned, Assigned> {
public:
	using child_ptr_first = child_ptr<Assigned, Auto>;
	using child_ptr_second = child_ptr<Assigned, Assigned>;

public:
	SplitLayout(child_ptr_first first, child_ptr_second second);

	// child
protected:
	child_ptr_first first;
	child_ptr_second second;

	// layout
protected:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
protected:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(size.width, length_first)); }
	Rect GetRegionSecond() const { return Rect(Point(0.0f, length_first), Size(size.width, length_second)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
protected:
	void UpdateLayout();
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


template<>
class SplitLayout<Vertical, Second> : public WndType<Assigned, Assigned> {
public:
	using child_ptr_first = child_ptr<Assigned, Assigned>;
	using child_ptr_second = child_ptr<Assigned, Auto>;

public:
	SplitLayout(child_ptr_first first, child_ptr_second second);

	// child
protected:
	child_ptr_first first;
	child_ptr_second second;

	// layout
protected:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
protected:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(size.width, length_first)); }
	Rect GetRegionSecond() const { return Rect(Point(0.0f, length_first), Size(size.width, length_second)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
protected:
	void UpdateLayout();
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


template<>
class SplitLayout<Horizontal, First> : public WndType<Assigned, Assigned> {
public:
	using child_ptr_first = child_ptr<Auto, Assigned>;
	using child_ptr_second = child_ptr<Assigned, Assigned>;

public:
	SplitLayout(child_ptr_first first, child_ptr_second second);

	// child
protected:
	child_ptr_first first;
	child_ptr_second second;

	// layout
private:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
private:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(length_first, size.height)); }
	Rect GetRegionSecond() const { return Rect(Point(length_first, 0.0f), Size(length_second, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
private:
	void UpdateLayout();
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


template<>
class SplitLayout<Horizontal, Second> : public WndType<Assigned, Assigned> {
public:
	using child_ptr_first = child_ptr<Assigned, Assigned>;
	using child_ptr_second = child_ptr<Auto, Assigned>;

public:
	SplitLayout(child_ptr_first first, child_ptr_second second);

	// child
protected:
	child_ptr_first first;
	child_ptr_second second;

	// layout
private:
	Size size;
	float length_first = 0.0f;
	float length_second = 0.0f;
private:
	Rect GetRegionFirst() const { return Rect(point_zero, Size(length_first, size.height)); }
	Rect GetRegionSecond() const { return Rect(Point(length_first, 0.0f), Size(length_second, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return &child == first.get() ? GetRegionFirst() : GetRegionSecond(); }
private:
	void UpdateLayout();
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) const override;

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)