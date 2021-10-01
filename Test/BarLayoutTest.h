#include "WndDesign/window/Global.h"
#include "WndDesign/widget/TitleBarFrame.h"
#include "WndDesign/frame/CenterFrame.h"
#include "WndDesign/control/ImageBox.h"


using namespace WndDesign;


struct MainFrameStyle : DesktopFrameStyle {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"BarLayoutTest");
	}
};


int main() {
	global.AddWnd(
		new TitleBarFrame{
			MainFrameStyle(),
			new CenterFrame<Assigned, Assigned>{
				new ImageBox(L"R:\\1.jpg")
			}
		}
	);
	global.MessageLoop();
}