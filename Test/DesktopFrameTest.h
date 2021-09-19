#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/control/Placeholder.h"
#include "WndDesign/figure/shape.h"
#include "WndDesign/system/win32_aero_snap.h"


using namespace WndDesign;


class EmptyWindow : public Placeholder<Relative, Relative> {
private:
	Color color = Color::Gray;
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const override {
		figure_queue.add(draw_region.point, new Rectangle(draw_region.size, color));
	}
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (msg.type) {
		case MouseMsg::Move: SetCursor(Cursor::Default); break;
		case MouseMsg::RightDown:
			SetFocus();
			color = color == Color::Gray ? Color::White : Color::Gray;
			Redraw(region_infinite);
			break;
		case MouseMsg::LeftDown: AeroSnapDraggingEffect(*this, msg.point); break;
		}
	}
	virtual void OnKeyMsg(KeyMsg msg) {
		if (msg.key == Key::Escape) {
			global.Terminate();
		}
	}
};


int main() {
	DesktopFrameStyle style;
	style.width.normal(800px).max(100pct);
	style.height.normal(500px).max(100pct);
	style.position.setHorizontalCenter().setVerticalCenter();
	style.border.width(5).color(Color::Violet);
	style.title.assign(L"DesktopTest");

	global.AddWnd(std::make_unique<DesktopFrame>(style, std::make_unique<EmptyWindow>()));
	global.MessageLoop();
}