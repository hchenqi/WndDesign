#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/frame/PaddingFrame.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/wrapper/Background.h"


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


class MyEditBox : public Decorate<EditBox, SolidColorBackground> {
private:
	struct Style : EditBox::Style {
		Style() {
			font.family(L"Calibri", L"DengXian").size(20);
		}
	};
protected:
	MyEditBox() : Base(Style(), L"Hello World!") {}
public:
	static auto Create() {
		return new ClipFrame<Assigned, Auto>(new PaddingFrame(Padding(100), new MyEditBox()));
	}
};


int main() {
	global.AddWnd(
		new MainFrame{
			L"EditBoxTest",
			new ScrollFrame{
				MyEditBox::Create()
			}
		}
	);
	global.MessageLoop();
}