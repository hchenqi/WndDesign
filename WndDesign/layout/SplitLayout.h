#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction, class SizeType>
class SplitLayout;


template<>
class SplitLayout<Vertical, Auto> : public WndType<Fixed, Auto> {
public:
	using child_ptr = child_ptr<Fixed, Auto>;

public:
	SplitLayout(child_ptr first, child_ptr second) : first(std::move(first)), second(std::move(second)) {
		RegisterChild(this->first);
		RegisterChild(this->second);
	}

protected:
	child_ptr first;
	child_ptr second;

private:
	virtual const Size OnSizeRefChange(Size size_ref) {
		return SetChildSizeRef(child, size_ref);
	}
	virtual void OnChildSizeChange(WndObject& child, Size child_size) {
		SizeChanged(child_size);
	}
};


template<>
class SplitLayout<Horizontal, Auto> : public WndObject {
public:
	SplitLayout(child_ptr left, child_ptr right) : left(std::move(left)), right(std::move(right)) {
		RegisterChild(this->left);
		RegisterChild(this->right);
	}

protected:
	child_ptr left;
	child_ptr right;
};


END_NAMESPACE(WndDesign)