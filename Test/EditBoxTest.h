#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/frame/PaddingFrame.h"
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
protected:
	MyEditBox() : Base(Style(), L"Hello World!") {}
public:
	static auto Create() {
		return new ClipFrame<Assigned, Auto>(new PaddingFrame(Padding(100), new MyEditBox()));
	}
};


int main() {
	global.AddWnd(
		new DesktopFrame{
			MainFrameStyle(),
			new ScrollFrame{
				MyEditBox::Create()
			}
		}
	);
	global.MessageLoop();
}