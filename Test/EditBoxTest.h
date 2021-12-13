#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/PaddingFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/wrapper/Background.h"


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


class MyEditBox : public Decorate<EditBox, SolidColorBackground> {
private:
	struct Style : EditBox::Style {
		Style() {
			font.family(L"Calibri", L"DengXian").size(20);
		}
	};
public:
	MyEditBox() : Base(Style(), L"Hello World!") {}
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