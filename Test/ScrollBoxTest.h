#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/BorderFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/widget/ScrollBox.h"
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


struct EditBoxStyle : EditBox::Style {
	EditBoxStyle() {
		font.family(L"DengXian").size(20).weight(FontWeight::Bold).color(Color::Black);
	}
};


int main() {
	global.AddWnd(
		new MainFrame{
			L"ScrollBoxTest",
			new BorderFrame{
				Border(3px, Color::Black),
				new ScrollBox{
					new ClipFrame<Assigned, Auto>{
						new EditBox(EditBoxStyle(), L"edit here...")
					}
				}
			}
		}
	);
	global.MessageLoop();
}