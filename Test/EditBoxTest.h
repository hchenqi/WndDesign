#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/PaddingFrame.h"
#include "WndDesign/control/EditBox.h"


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
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(draw_region.point, new Rectangle(draw_region.size, Color::White));
		EditBox::OnDraw(figure_queue, draw_region);
	}
};


int main() {
	global.AddWnd(
		new DesktopFrame{
			MainFrameStyle(),
			new ScrollFrame<Vertical>{
				new PaddingFrame<Assigned, Auto>{
					Margin(100),
					new MyEditBox()
				}
			}
		}
	);
	global.MessageLoop();
}