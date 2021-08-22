#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction, class SizeType>
class SplitLayout;


template<>
class SplitLayout<Vertical, Auto> : public WndType<Relative, Auto> {
public:
	using child_ptr = child_ptr<Relative, Auto>;
public:
	SplitLayout(child_ptr first, child_ptr second) : first(std::move(first)), second(std::move(second)) {
		RegisterChild(this->first);
		RegisterChild(this->second);
	}
protected:
	child_ptr first;
	child_ptr second;
private:
	uint width;
	uint height_first;
	uint height_second;
private:
	const Size GetSize() const { return Size(width, height_first + height_second); }
private:
	virtual const Size OnSizeRefChange(Size size_ref) override {
		if (width != size_ref.width) {
			width = size_ref.width;
			height_first = SetChildSizeRef(first, Size(width, length_min)).height;
			height_second = SetChildSizeRef(second, Size(width, length_min)).height;
		}
		return GetSize();
	}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) override {
		if (&child == first.get()) {
			if (height_first != child_size.height) {
				height_first = child_size.height;
				SizeChanged(GetSize());
			}
		} else {
			if (height_second != child_size.height) {
				height_second = child_size.height;
				SizeChanged(GetSize());
			}
		}
	}
};


END_NAMESPACE(WndDesign)