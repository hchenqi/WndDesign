#pragma once

#include "../window/wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ListLayout;


template<>
class ListLayout<Vertical> : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Assigned, Auto>;

public:
	template<class... Ts>
	ListLayout(float gap, Ts... child_args) : child_list(), gap(gap) {
		child_list.reserve(sizeof...(Ts)); (child_list.emplace_back(std::move(child_args)), ...);
		uint index = 0;	for (auto& info : child_list) { RegisterChild(info.child); SetChildData(info.child, index++); }
	}

	// child
protected:
	struct ChildInfo {
	public:
		child_ptr child;
		float offset = 0.0f;
		float length = 0.0f;
	public:
		ChildInfo(child_ptr child) : child(std::move(child)) {}
	public:
		float BeginOffset() const { return offset; }
		float EndOffset() const { return offset + length; }
	};
	std::vector<ChildInfo> child_list;
protected:
	void SetChildData(WndObject& child, size_t index) { WndObject::SetChildData<size_t>(child, index); }
	size_t GetChildData(WndObject& child) const { return WndObject::GetChildData<size_t>(child); }
public:
	void InsertChild(size_t index, child_ptr child);
	void EraseChild(size_t begin, size_t count);
	void AppendChild(child_ptr child) { InsertChild(-1, std::move(child)); }

	// layout
protected:
	Size size;
	float gap;
protected:
	Rect GetChildRegion(size_t index) const { return Rect(Point(0.0f, child_list[index].offset), Size(size.width, child_list[index].length)); }
	Rect GetChildRegion(WndObject& child) const { return GetChildRegion(GetChildData(child)); }
	auto HitTestItem(float offset) const;
protected:
	void UpdateLayout(size_t index);
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
class ListLayout<Horizontal> : public WndType<Auto, Assigned> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;

public:
	template<class... Ts>
	ListLayout(float gap, Ts... child_args) : child_list(), gap(gap) {
		child_list.reserve(sizeof...(Ts)); (child_list.emplace_back(std::move(child_args)), ...);
		uint index = 0;	for (auto& info : child_list) { RegisterChild(info.child); SetChildData(info.child, index++); }
	}

	// child
protected:
	struct ChildInfo {
	public:
		child_ptr child;
		float offset = 0.0f;
		float length = 0.0f;
	public:
		ChildInfo(child_ptr child) : child(std::move(child)) {}
	public:
		float BeginOffset() const { return offset; }
		float EndOffset() const { return offset + length; }
	};
	std::vector<ChildInfo> child_list;
protected:
	void SetChildData(WndObject& child, size_t index) { WndObject::SetChildData<size_t>(child, index); }
	size_t GetChildData(WndObject& child) const { return WndObject::GetChildData<size_t>(child); }
public:
	void InsertChild(size_t index, child_ptr child);
	void EraseChild(size_t begin, size_t count);
	void AppendChild(child_ptr child) { InsertChild(-1, std::move(child)); }

	// layout
protected:
	Size size;
	float gap;
protected:
	Rect GetChildRegion(size_t index) const { return Rect(Point(child_list[index].offset, 0.0f), Size(child_list[index].length, size.height)); }
	Rect GetChildRegion(WndObject& child) const { return GetChildRegion(GetChildData(child)); }
	auto HitTestItem(float offset) const;
protected:
	void UpdateLayout(size_t index);
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