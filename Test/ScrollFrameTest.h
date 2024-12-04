#include "WndDesign/frame/PaddingFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/frame/ScaleFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/wrapper/Background.h"
#include "WndDesign/window/Global.h"


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


class ScaleLayer : public ScaleFrame<Assigned, Auto> {
public:
	ScaleLayer(child_ptr child) : ScaleFrame(Scale(1.0), std::move(child)) {}
private:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (msg.ctrl && msg.type == MouseMsg::WheelVertical) {
			return this;
		}
		return ScaleFrame::HitTest(msg);
	}
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		SetScale(scale * Scale(powf(1.1f, msg.wheel_delta / 120.0f)));
	}
};


class MyEditBox : public Decorate<EditBox, SolidColorBackground> {
private:
	struct Style : EditBox::Style {
		Style() {
			font.family(L"Calibri").size(20);
		}
	};
protected:
	MyEditBox() : Base(Style(), L"Hello World!") {}
public:
	static auto Create() {
		return new ClipFrame<Assigned, Auto>(new PaddingFrame(Padding(30px), new MyEditBox()));
	}
};


int main() {
	global.AddWnd(
		new MainFrame(
			L"ScrollFrameTest",
			new ScrollFrame<Vertical>(
				new ScaleLayer(
					MyEditBox::Create()
				)
			)
		)
	);
	global.MessageLoop();
}