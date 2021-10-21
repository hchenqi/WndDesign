#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/PaddingFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/control/EditBox.h"


using namespace WndDesign;


struct MainFrameStyle : DesktopFrame::Style {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"EditBoxTest");
	}
};


class MyEditBox : public EditBox {
private:
	struct Style : EditBox::Style {
		Style() {
			font.family(L"Calibri", L"DengXian").size(20);
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
					new ClipFrame<Assigned, Auto>{
						new MyEditBox()
					}
				}
			}
		}
	);
	global.MessageLoop();
}