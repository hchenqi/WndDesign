#pragma once

#include "../frame/ScrollFrame.h"
#include "../control/Scrollbar.h"
#include "../layout/SplitLayout.h"


BEGIN_NAMESPACE(WndDesign)


class ScrollBox : public SplitLayout<Horizontal, Second> {
public:
	using child_ptr = ScrollFrame<Vertical>::child_ptr;
public:
	ScrollBox(child_ptr child) : SplitLayout(new Frame(std::move(child)), new Bar()) {}
private:
	class Frame : public ScrollFrame<Vertical> {
		using ScrollFrame::ScrollFrame;
		ScrollBox& GetScrollbox() const { return static_cast<ScrollBox&>(GetParent()); }
		virtual void OnFrameOffsetUpdate(uint content_height, uint frame_height, int frame_offset) override {
			GetScrollbox().GetBar().UpdateScrollOffset(content_height, frame_height, frame_offset);
		}
	};
	class Bar : public Scrollbar<Vertical> {
		ScrollBox& GetScrollbox() const { return static_cast<ScrollBox&>(GetParent()); }
		virtual void OnFrameOffsetChange(int scroll_offset) override {
			GetScrollbox().GetFrame().UpdateFrameOffset(scroll_offset);
		}
		virtual void OnMouseMsg(MouseMsg msg) override {
			if (msg.type == MouseMsg::WheelVertical) { GetScrollbox().GetFrame().Scroll(-msg.wheel_delta); }
		}
	};
private:
	Frame& GetFrame() const { return static_cast<Frame&>(*first); }
	Bar& GetBar() const { return static_cast<Bar&>(*second); }
};


END_NAMESPACE(WndDesign)