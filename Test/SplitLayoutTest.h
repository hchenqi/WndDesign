#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/layout/SplitLayout.h"
#include "WndDesign/control/EditBox.h"


using namespace WndDesign;


struct MainFrameStyle : DesktopFrame::Style {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"SplitLayoutTest");
	}
};


struct EditBoxStyle : EditBox::Style {
	EditBoxStyle() {
		font.family(L"DengXian").size(20).weight(FontWeight::Bold).color(Color::Black);
	}
};


struct TextBoxStyle : TextBlockStyle {
	TextBoxStyle() {
		font.family(L"Segoe UI").size(100).color(Color::Lime);
	}
};


int main() {
	global.AddWnd(
		new DesktopFrame{
			MainFrameStyle(),
			new SplitLayoutVertical{
				new ClipFrame<Assigned, Auto>{
					new EditBox(EditBoxStyle(), L"edit here...")
				},
				new ClipFrame<>{
					new TextBox(TextBoxStyle(), L"Hello World!")
				}
			}
		}
	);
	global.MessageLoop();
}