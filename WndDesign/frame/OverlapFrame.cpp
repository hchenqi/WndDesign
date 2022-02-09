#include "OverlapFrame.h"
#include "../style/style_helper.h"


BEGIN_NAMESPACE(WndDesign)


Rect OverlapFrame::UpdateLayout(Size size, Size size_ref) {
	PositionStyle position = this->position;
	Rect region_new;
	region_new.point.x = StyleHelper::CalculatePosition(position._left.ConvertToPixel(size_ref.width), position._right.ConvertToPixel(size_ref.width), size.width, size_ref.width);
	region_new.point.y = StyleHelper::CalculatePosition(position._top.ConvertToPixel(size_ref.height), position._bottom.ConvertToPixel(size_ref.height), size.height, size_ref.height);
	region_new.size = size;
	if (region != region_new) {
		Rect redraw_region = region.Union(region_new);
		region = region_new;
		return redraw_region;
	} else {
		return region_empty;
	}
}


END_NAMESPACE(WndDesign)