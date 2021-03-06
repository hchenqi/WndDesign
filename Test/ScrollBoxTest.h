#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/widget/ScrollBox.h"
#include "WndDesign/control/EditBox.h"


using namespace WndDesign;


struct MainFrameStyle : DesktopFrame::Style {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"ScrollBoxTest");
	}
};


struct EditBoxStyle : EditBox::Style {
	EditBoxStyle() {
		font.family(L"DengXian").size(20).weight(FontWeight::Bold).color(Color::Black);
	}
};


int main() {
	global.AddWnd(
		new DesktopFrame{
			MainFrameStyle(),
			new ScrollBox{
				new ClipFrame<Assigned, Auto>{
					new EditBox(EditBoxStyle(), L"edit here...")
				}
			}
		}
	);
	global.MessageLoop();
}