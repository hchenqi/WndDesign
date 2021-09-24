#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/MarginFrame.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/figure/shape.h"


using namespace WndDesign;


struct MainFrameStyle : DesktopFrameStyle {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"DesktopTest");
	}
};


class MyEditBox : public EditBox {
private:
	struct Style : EditBox::Style {
		Style() {
			font.family(L"DengXian").size(20).weight(FontWeight::Bold).color(Color::Black);
		}
	};
public:
	MyEditBox() : EditBox(Style(), L"Hello World!") {}
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) {
		figure_queue.add(draw_region.point, new Rectangle(draw_region.size, Color::White));
		EditBox::OnDraw(figure_queue, draw_region);
	}
};


class MyEditBoxMargin : public MarginFrame, public LayoutType<Assigned, Auto> {
private:
	struct Style : MarginFrame::Style {
		Style() {
			border.color(Color::DarkGreen).width(10).radius(20);
			padding.set(30);
		}
	};
public:
	using child_ptr = WndDesign::child_ptr<Assigned, Auto>;
public:
	MyEditBoxMargin(child_ptr child) : MarginFrame(Style(), std::move(child)) {}
};


int main() {
	global.AddWnd(new DesktopFrame(MainFrameStyle(), new ScrollFrame<Vertical>(new MyEditBoxMargin(new MyEditBox()))));
	global.MessageLoop();
}