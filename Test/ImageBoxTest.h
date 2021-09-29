#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/CenterFrame.h"
#include "WndDesign/layout/SplitLayout.h"
#include "WndDesign/control/ImageBox.h"


using namespace WndDesign;


struct MainFrameStyle : DesktopFrameStyle {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"ImageBoxTest");
	}
};


int main() {
	global.AddWnd(
		new DesktopFrame{
			MainFrameStyle(),
			new SplitLayout<Vertical, First>{
				new CenterFrame<Assigned, Auto>{
					new ImageBox(L"R:\\1.jpg")
				},
				new ImageRepeatBox(L"R:/2.gif", Point(50, 50))
			}
		}
	);
	global.MessageLoop();
}