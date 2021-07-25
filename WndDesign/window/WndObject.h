#pragma once

#include "../common/uncopyable.h"
#include "../geometry/geometry.h"


BEGIN_NAMESPACE(WndDesign)


class WndObject : Uncopyable {

protected:
	WndObject() {

	}
	~WndObject() {

	}


private:
	ref_ptr<WndObject> parent = nullptr;

private:
	uint64 parent_specific_data = 0;

	GetChildData(WndObject& child);

	bool HasParent() const { return parent != nullptr; }

	WndObject& GetParent() const { 
		if (!HasParent()) { throw std::invalid_argument("window has no parent"); } 
		return *parent; 
	}

private:
	Size size = size_empty;

public:
	const Size GetSize() const { return size; }

private:
	void SetSize(Size size) { this->size = size; }

private:
	bool size_invalid = false;
	bool layout_invalid = false;

	void InvalidateSize() {}
	void InvalidateLayout() {}

public:
	bool IsSizeInvalid() const { return size_invalid; }
	bool IsLayoutInvalid() const { return layout_invalid; }

	bool IsSizeAuto() const {}
	bool IsSizeRelative() const {}
	bool IsLayoutRelative() const {}
	bool IsLayoutAuto() const {}
	bool IsLayoutStrict() const {}

	friend class ReflowQueue;
	friend class RedrawQueue;


	const Size UpdateSize(Size size) {
		size = UpdateLayout(size);
		SetSize(size);
		size_invalid = false;
		layout_invalid = false;
		return size;
	}


	virtual const Size UpdateLayout(Size size);


	virtual void InvalidateChildLayout(WndObject& child);


private:
	bool region_invalid = false;
	Rect invalid_region = region_empty;


public:
	bool HasInvalidRegion() const { return invalid_region != region_empty; }
	const Rect GetInvalidRegion() const { return invalid_region; }

	void InvalidateChildRegion(WndObject& child) {
		Rect child_region = GetChildRegion(child);
		InvalidateRegion(child_region.Intersect(child.GetInvalidRegion() + (child_region.point - point_zero)));
	}

	virtual const Rect GetChildRegion(WndObject& child) { return region_empty; }

	void InvalidateRegion(Rect invalid_region) {
		this->invalid_region = this->invalid_region.Union(invalid_region);
		if (!region_invalid) {
			region_invalid = true;
			GetRedrawQueue().AddWnd(*this);
		}
	}

	virtual void OnPaint(FigureQueue& figure_queue, Rect invalid_region);


private:
	virtual void Handler();
};


END_NAMESPACE(WndDesign)