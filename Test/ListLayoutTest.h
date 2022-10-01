#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/layout/ListLayout.h"
#include "WndDesign/control/EditBox.h"


using namespace WndDesign;


class MainFrame : public DesktopFrame {
public:
	using DesktopFrame::DesktopFrame;
private:
	Size size = Size(800, 500);
private:
	virtual std::pair<Size, Size> CalculateMinMaxSize(Size size_ref) {
		return { Size(100, 100), size_ref };
	}
	virtual Rect OnDesktopFrameSizeRefUpdate(Size size_ref) override {
		Rect region;
		region.size = UpdateChildSizeRef(child, size);
		region.point.x = (size_ref.width - region.size.width) / 2;
		region.point.y = (size_ref.height - region.size.height) / 2;
		return region;
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
		new MainFrame{
			L"ListLayoutTest",
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
						new EditBox(EditBoxStyle2(), L"EditBox2")
					}
				}
			}
		}
	);
	global.MessageLoop();
}