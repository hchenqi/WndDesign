#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/frame/InnerBorderFrame.h"
#include "WndDesign/control/TextBox.h"
#include "WndDesign/style/border_style.h"
#include "WndDesign/geometry/helper.h"
#include "WndDesign/system/cursor.h"
#include "WndDesign/system/win32_aero_snap.h"


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


class ResizeBorder : public InnerBorderFrame<Assigned, Assigned> {
public:
	ResizeBorder(child_ptr child) : InnerBorderFrame<Assigned, Assigned>(Border(5.0, Color(Color::Yellow, 127)), std::move(child)) {}
private:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (PointInRoundedRectangle(msg.point, Extend(Rect(point_zero, size), -border._width), border._radius)) { return child; }
		return this;
	}
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::Move || msg.type == MouseMsg::LeftDown) {
			BorderPosition border_position = HitTestBorderPosition(size, border._width + border._radius, msg.point);
			if (msg.type == MouseMsg::Move) {
				SetCursor(GetBorderPositionCursor(border_position));
			} else {
				AeroSnapBorderResizingEffect(*this, border_position);
			}
		}
	}
};


struct TextBoxStyle : TextBlockStyle {
	TextBoxStyle() {
		font.family(L"Segoe UI").size(75).color(Color::Lime);
	}
};


int main() {
	global.AddWnd(
		new MainFrame{
			L"TextBoxTest",
			new ResizeBorder{
				new ClipFrame<>{
					new InnerBorderFrame{
						Border(1.0, Color::Black),
						new TextBox(TextBoxStyle(), L"Hello World!")
					}
				}
			}
		}
	);
	global.MessageLoop();
}