#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/CenterFrame.h"
#include "WndDesign/layout/SplitLayout.h"
#include "WndDesign/control/ImageBox.h"


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


int main() {
	global.AddWnd(
		new MainFrame{
			L"ImageBoxTest",
			new SplitLayoutVertical{
				new CenterFrame<Assigned, Auto>{
					new ImageBox(L"R:\\1.jpg")
				},
				new ImageRepeatBox(L"R:/2.gif", Point(50, 50))
			}
		}
	);
	global.MessageLoop();
}