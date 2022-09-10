#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/control/TextBox.h"


using namespace WndDesign;


struct MainFrameStyle : DesktopFrame::Style {
	MainFrameStyle() {
		width.min(100px).normal(800px).max(100pct);
		height.min(100px).normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Gray);
		title.assign(L"TextBoxTest");
	}
};


struct TextBoxStyle : TextBlockStyle {
	TextBoxStyle() {
		font.family(L"Segoe UI").size(75).color(Color::Lime);
	}
};


int main() {
	global.AddWnd(
		new DesktopFrame{
			MainFrameStyle(),
			new ClipFrame<>{
				new BorderFrame{
					Border(1.0, Color::Black),
					new TextBox(TextBoxStyle(), L"Hello World!")
				}
			}
		}
	);
	global.MessageLoop();
}