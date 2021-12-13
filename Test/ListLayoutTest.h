#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/PaddingFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/layout/ListLayout.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/figure/shape.h"


using namespace WndDesign;


struct MainFrameStyle : DesktopFrame::Style {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"ListLayoutTest");
	}
};


struct TextBoxStyle : TextBox::Style {
	TextBoxStyle() {
		paragraph.line_height(200pct);
		font.family(L"DengXian").size(20).color(Color::Moccasin);
	}
};

struct EditBoxStyle1 : EditBox::Style {
	EditBoxStyle1() {
		font.family(L"Calibri").size(60).color(Color::SeaGreen);
	}
};

struct EditBoxStyle2 : EditBox::Style {
	EditBoxStyle2() {
		font.family(L"Segoe UI").size(30).style(FontStyle::Italic).color(Color::Fuchsia);
	}
};


int main() {
	global.AddWnd(
		new DesktopFrame{
			MainFrameStyle(),
			new ScrollFrame{
				new ListLayout<Vertical>{
					100,
					new ClipFrame<Assigned, Auto>{
						new TextBox(TextBoxStyle(), L"ListLayoutTest"),
					},
					new ClipFrame<Assigned, Auto>{
						new EditBox(EditBoxStyle1(), L"EditBox")
					},
					new ClipFrame<Assigned, Auto>{
						new PaddingFrame{
							Margin(20, 0),
							new EditBox(EditBoxStyle2(), L"EditBoxWithPadding")
						}
					}
				}
			}
		}
	);
	global.MessageLoop();
}