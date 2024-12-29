#pragma once

#include "../frame/WndFrame.h"


BEGIN_NAMESPACE(WndDesign)


template<class Frame> requires std::is_base_of_v<WndFrame, Frame>
class MouseThrough : public Frame {
public:
	using Base = MouseThrough;

public:
	using Frame::Frame;

protected:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (Frame::GetChildRegion().Contains(msg.point)) {
			msg.point -= Frame::GetChildOffset();
			return WndFrame::HitTest(msg);
		}
		return nullptr;
	}
};


END_NAMESPACE(WndDesign)