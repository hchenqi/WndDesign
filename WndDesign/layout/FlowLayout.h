#pragma once

#include "../window/wnd_traits.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


class FlowLayout : public WndType<Assigned, Auto> {
public:
	using child_ptr = child_ptr<Auto, Assigned>;

public:
	FlowLayout(uint row_height, uint column_gap, uint row_gap);

	// style
private:
	uint row_height;
	uint column_gap;
	uint row_gap;

	// child
private:
	struct ChildInfo {
		child_ptr child;
		uint width = 0;
		uint offset = 0;
		ChildInfo(child_ptr child) : child(std::move(child)) {}
	};
	std::vector<ChildInfo> child_list;
private:
	using child_index = uint;
	std::vector<child_index> row_list;
private:
	using row_index = uint; using column_index = uint;
	using ChildData = std::pair<row_index, column_index>;
private:
	void SetChildData(WndObject& child, ChildData data) { WndObject::SetChildData<ChildData>(child, data); }
	ChildData GetChildData(WndObject& child) const { return WndObject::GetChildData<ChildData>(child); }
private:
	Rect GetChildRegion(WndObject& child) const;
	uint GetRowOffset(uint row) const;
	uint GetRowNumber() const;
	uint HitTestRow(uint y) const;
	auto HitTestColumn(row_index row, uint x) const;
public:
	void AppendChild(child_ptr child);


	// layout
private:
	Size size;
private:
	bool UpdateLayout(child_index child_index);
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override;
	virtual void OnChildSizeUpdate(WndObject& child, Size child_size) override;
	virtual Size GetSize() override { return size; }
protected:
	virtual ref_ptr<WndObject> HitTest(Point& point) override;
	virtual Transform GetChildTransform(WndObject& child) override;

	// paint
private:
	Rect redraw_region;
protected:
	virtual Rect GetRedrawRegion() override { return redraw_region; }
	virtual void OnChildRedraw(WndObject& child, Rect child_redraw_region) override;
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;
};


END_NAMESPACE(WndDesign)