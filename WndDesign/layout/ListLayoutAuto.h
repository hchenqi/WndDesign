#pragma once

#include "../window/wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ListLayoutAuto;


class _ListLayoutAuto_Base : public WndType<Auto, Auto> {
public:
	using child_ptr = child_ptr<Auto, Auto>;

protected:
	_ListLayoutAuto_Base(float gap) : child_list(), gap(gap) {}

	// child
protected:
	struct ChildInfo {
	public:
		child_ptr child;
		Rect region;
	public:
		ChildInfo(child_ptr child) : child(std::move(child)) {}
	};
	std::vector<ChildInfo> child_list;
protected:
	using child_iter = std::vector<ChildInfo>::iterator;
protected:
	void SetChildIndex(WndObject& child, size_t index) { WndObject::SetChildData<size_t>(child, index); }
	void UpdateIndex(size_t begin) {
		for (size_t index = begin; index < child_list.size(); ++index) {
			SetChildIndex(child_list[index].child, index);
		}
	}
public:
	size_t Length() const { return child_list.size(); }
	WndObject& GetChild(size_t index) const { return child_list[index].child; }
	size_t GetChildIndex(WndObject& child) const { return WndObject::GetChildData<size_t>(child); }
protected:
	void InsertChild(size_t& index, child_ptr& child);
	void InsertChild(size_t& begin, std::vector<child_ptr>& children);
	void EraseChild(size_t begin, size_t count);
	child_ptr ExtractChild(size_t index);
	std::vector<child_ptr> ExtractChild(size_t begin, size_t count);

	// layout
protected:
	Size size;
	float gap = 0.0f;
protected:
	Rect GetChildRegion(size_t index) const { return child_list[index].region; }
	Rect GetChildRegion(WndObject& child) const { return GetChildRegion(GetChildIndex(child)); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { return size; }
	virtual Transform GetChildTransform(WndObject& child) const override { return GetChildRegion(child).point - point_zero; }

	// paint
protected:
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override {
		Rect child_region = GetChildRegion(child);
		Redraw(child_region.Intersect(child_redraw_region + (child_region.point - point_zero)));
	}
};


template<>
class ListLayoutAuto<Vertical> : public _ListLayoutAuto_Base {
public:
	template<class... Ts>
	ListLayoutAuto(float gap, Ts... child_args) : _ListLayoutAuto_Base(gap) {
		if constexpr (sizeof...(Ts) > 0) {
			child_ptr list[] = { std::move(child_args)... };
			InsertChild(0, std::vector<child_ptr>(std::make_move_iterator(std::begin(list)), std::make_move_iterator(std::end(list))));
		}
	}

	// child
public:
	void InsertChild(size_t index, child_ptr child);
	void InsertChild(size_t begin, std::vector<child_ptr> children);
	void AppendChild(child_ptr child) { InsertChild(-1, std::move(child)); }
	void EraseChild(size_t begin, size_t count = 1);
	child_ptr ExtractChild(size_t index);
	std::vector<child_ptr> ExtractChild(size_t begin, size_t count);

	// layout
protected:
	child_iter HitTestItem(float offset);
	void UpdateLayout(size_t index);
protected:
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
	virtual ref_ptr<WndObject> HitTest(Point& point) override;

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


template<>
class ListLayoutAuto<Horizontal> : public _ListLayoutAuto_Base {
public:
	template<class... Ts>
	ListLayoutAuto(float gap, Ts... child_args) : _ListLayoutAuto_Base(gap) {
		if constexpr (sizeof...(Ts) > 0) {
			child_ptr list[] = { std::move(child_args)... };
			InsertChild(0, std::vector<child_ptr>(std::make_move_iterator(std::begin(list)), std::make_move_iterator(std::end(list))));
		}
	}

	// child
public:
	void InsertChild(size_t index, child_ptr child);
	void InsertChild(size_t begin, std::vector<child_ptr> children);
	void AppendChild(child_ptr child) { InsertChild(-1, std::move(child)); }
	void EraseChild(size_t begin, size_t count = 1);
	child_ptr ExtractChild(size_t index);
	std::vector<child_ptr> ExtractChild(size_t begin, size_t count);

	// layout
protected:
	child_iter HitTestItem(float offset);
	void UpdateLayout(size_t index);
protected:
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
	virtual ref_ptr<WndObject> HitTest(Point& point) override;

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)