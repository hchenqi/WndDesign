#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/control/TextBox.h"


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


struct TextBoxStyle : TextBlockStyle {
	TextBoxStyle() {
		font.family(L"Segoe UI").size(100).color(Color::Lime);
	}
};


int main() {
	global.AddWnd(new DesktopFrame(MainFrameStyle(), new ClipFrame<Vertical>(new TextBox(TextBoxStyle(), L"Hello World!"))));
	global.MessageLoop();
}