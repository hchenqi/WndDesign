#include "WndDesign/window/Global.h"
#include "WndDesign/layout/OverlapLayout.h"
#include "WndDesign/frame/BorderFrame.h"
#include "WndDesign/widget/TitleBarFrame.h"
#include "WndDesign/message/mouse_tracker.h"
#include "WndDesign/wrapper/Background.h"


using namespace WndDesign;


struct MainFrameStyle : TitleBarFrame::Style {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"OverlapLayoutTest");
	}
};


class Container : public OverlapLayout {
public:
	Container() {
		AddChild(new Frame(new BorderFrame(Border(5px, Color::Wheat), new Wnd(Color(Color::Indigo, 127)))));
		AddChild(new Frame(new BorderFrame(Border(10px, 20px, Color::Goldenrod), new Wnd(Color(Color::GreenYellow, 63)))));
	}

private:
	class Frame : public OverlapFrame {
	public:
		Frame(child_ptr child) : OverlapFrame(PositionStyle(position_center, position_center), std::move(child)) {}
	private:
		virtual ref_ptr<WndObject> HitTest(Point& point) override { return this; }
	private:
		MouseTracker mouse_tracker;
	private:
		virtual void OnMouseMsg(MouseMsg msg) override {
			switch (msg.type) {
			case MouseMsg::LeftDown: SetCapture(); break;
			case MouseMsg::LeftUp: ReleaseCapture(); break;
			}
			switch (mouse_tracker.Track(msg)) {
			case MouseTrackMsg::LeftDrag: {
				Point position_new = GetRegion().point + (msg.point - mouse_tracker.mouse_down_position);
				position.left(px(position_new.x)).top(px(position_new.y)); SizeUpdated(GetRegion().size);
			}
			}
		}
	};

	class Wnd : public Decorate<WndType<Relative, Relative>, SolidColorBackground> {
	public:
		Wnd(Color background_color) { background = background_color; }
	private:
		virtual Size OnSizeRefUpdate(Size size_ref) override {
			//width.normal(50pct).max(100pct);
			//height.normal(500px).max(100pct);
			return Size(size_ref.width / 2, size_ref.height >= 500px ? 500px : size_ref.height);
		}
	};
};


int main() {
	global.AddWnd(
		new TitleBarFrame{
			MainFrameStyle(),
			new Container
		}
	);
	global.MessageLoop();
}