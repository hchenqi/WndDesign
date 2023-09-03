#include "WndDesign/window/Global.h"
#include "WndDesign/layout/OverlapLayout.h"
#include "WndDesign/frame/BorderFrame.h"
#include "WndDesign/widget/TitleBarFrame.h"
#include "WndDesign/message/mouse_tracker.h"
#include "WndDesign/wrapper/Background.h"


using namespace WndDesign;


struct MainFrameStyle : TitleBarFrame::Style {
	MainFrameStyle() {
		title.assign(L"OverlapLayoutTest");
		background_color = color_transparent;
	}
};


class MainWindow : public OverlapLayout {
public:
	MainWindow() {}

private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (msg.type) {
		case MouseMsg::LeftDown: AddChild(new Frame(msg.point)); break;
		}
	}

private:
	class Frame : public OverlapFrame {
	public:
		Frame(Point point) : OverlapFrame{
			new BorderFrame{
				Border(5px, Color::Wheat),
				new Wnd(Color(Color::Indigo, 64))
			}
		}, region(point, Size(300, 300)) {
			UpdateChildSizeRef(child, region.size);
		}

		// layout
	private:
		Rect region;
	private:
		virtual Rect OnOverlapFrameSizeRefUpdate(Size size_ref) override { return region; }

		// message
	private:
		MouseTracker mouse_tracker;
	private:
		virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override { return this; }
	private:
		virtual void OnMouseMsg(MouseMsg msg) override {
			switch (msg.type) {
			case MouseMsg::LeftDown: SetCapture(); break;
			case MouseMsg::LeftUp: ReleaseCapture(); break;
			}
			switch (mouse_tracker.Track(msg)) {
			case MouseTrackMsg::LeftDrag:
				region.point += msg.point - mouse_tracker.mouse_down_position;
				OverlapFrameRegionUpdated(region);
				break;
			case MouseTrackMsg::LeftDoubleClick:
				GetParent().RemoveChild(*this);
				break;
			}
		}
	};

	class Wnd : public Decorate<Placeholder<Assigned, Assigned>, SolidColorBackground> {
	public:
		Wnd(Color background_color) { background = background_color; }
	};
};


int main() {
	global.AddWnd(
		new TitleBarFrame{
			MainFrameStyle(),
			new MainWindow
		}
	);
	global.MessageLoop();
}