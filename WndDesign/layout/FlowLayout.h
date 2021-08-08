#pragma once

#include "../window/WndObject.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)


class FlowLayoutLeftRightTopBottom : public WndObject {


private:
	struct ChildInfo {
		ref_ptr<WndObject> child;
		uint row_offset;
	};
	std::vector<ChildInfo> child_list;
	
	struct RowInfo {

	};

	std::vector<uint> row_list;

private:
	uint width_max;

private:
	virtual const Size OnSizeRefChange(Size size_ref) override {
		if (width_max != size_ref.width) {
			width_max = size_ref.width;

		}
		return size;
	}

	virtual void OnChildSizeChange(WndObject& child, Size child_size) {

	}

};


END_NAMESPACE(WndDesign)