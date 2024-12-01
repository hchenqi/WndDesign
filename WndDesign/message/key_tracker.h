#pragma once

#include "key_msg.h"
#include "../common/uncopyable.h"


BEGIN_NAMESPACE(WndDesign)


class KeyTracker : Uncopyable {
public:
	bool ctrl = false;
	bool shift = false;
public:
	void Track(KeyMsg msg) {
		switch (msg.type) {
		case KeyMsg::KeyDown:
			switch (msg.key) {
			case Key::Ctrl: ctrl = true; break;
			case Key::Shift: shift = true; break;
			}
			break;
		case KeyMsg::KeyUp:
			switch (msg.key) {
			case Key::Ctrl: ctrl = false; break;
			case Key::Shift: shift = false; break;
			}
			break;
		}
	}
};


END_NAMESPACE(WndDesign)