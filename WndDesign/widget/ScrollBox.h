#pragma once

#include "../frame/ScrollFrame.h"
#include "../control/Scrollbar.h"
#include "../layout/SplitLayout.h"


BEGIN_NAMESPACE(WndDesign)


template<class Direction>
class ScrollBox;


template<>
class ScrollBox<Vertical> : public SplitLayout<Horizontal, Second> {
protected:
	using ScrollFrame = ScrollFrame<Vertical>;
	using Scrollbar = Scrollbar<Vertical>;

public:
	using child_ptr = ScrollFrame::child_ptr;

public:
	ScrollBox(child_ptr child) : SplitLayout{ new Frame(std::move(child)), new Scrollbar() } {
		GetScrollbar().SetScrollFrame(GetScrollFrame());
	}

protected:
	class Frame : public ScrollFrame {
		using ScrollFrame::ScrollFrame;
		ScrollBox& GetScrollbox() const { return static_cast<ScrollBox&>(GetParent()); }
		virtual void OnFrameOffsetUpdate() override { GetScrollbox().GetScrollbar().UpdateScrollOffset(); }
	};

protected:
	ScrollFrame& GetScrollFrame() const { return static_cast<ScrollFrame&>(*first); }
	Scrollbar& GetScrollbar() const { return static_cast<Scrollbar&>(*second); }
};


template<class T>
ScrollBox(T)->ScrollBox<typename deduce_scroll_frame_direction<extract_width_type<T>, extract_height_type<T>>::type>;


END_NAMESPACE(WndDesign)