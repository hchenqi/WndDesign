#pragma once

#include "../frame/DesktopWndFrame.h"


BEGIN_NAMESPACE(WndDesign)


class Desktop final : public WndObject {
public:
	using child_ptr = std::unique_ptr<DesktopWndFrame>;



	void AddChild(child_ptr frame) {

	}


};


extern Desktop desktop;


END_NAMESPACE(WndDesign)