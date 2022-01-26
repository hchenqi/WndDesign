#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/control/Placeholder.h"
#include "WndDesign/wrapper/Background.h"
#include "WndDesign/system/win32_aero_snap.h"


using namespace WndDesign;


class EmptyWindow : public Decorate<Placeholder<Assigned, Assigned>, SolidColorBackground> {
public:
	EmptyWindow() {
		cursor = Cursor::Hand;
		background = Color::Gray;
	}
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (msg.type) {
		case MouseMsg::RightDown:
			SetFocus();
			background = background == Color::Gray ? Color::White : Color::Gray;
			Redraw();
			break;
		case MouseMsg::LeftDown: AeroSnapDraggingEffect(*this, msg.point); break;
		}
	}
	virtual void OnKeyMsg(KeyMsg msg) override {
		if (msg.key == Key::Escape) {
			global.Terminate();
		}
	}
};


int main() {
	DesktopFrame::Style style;
	style.width.normal(800px).max(100pct);
	style.height.normal(500px).max(100pct);
	style.position.setHorizontalCenter().setVerticalCenter();
	style.border.width(5).color(Color::Violet);
	style.title.assign(L"DesktopFrameTest");

	global.AddWnd(new DesktopFrame(style, new EmptyWindow()));
	global.MessageLoop();
}