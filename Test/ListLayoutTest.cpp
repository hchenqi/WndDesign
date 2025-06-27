#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/frame/InnerBorderFrame.h"
#include "WndDesign/layout/ListLayout.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/wrapper/HitTestHelper.h"


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


template<class InnerBorderFrame>
class HighlightFocus : public InnerBorderFrame {
public:
	HighlightFocus(InnerBorderFrame::child_type child) : InnerBorderFrame(Border(1.0f, Color::Black), std::move(child)) {}
private:
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		switch (msg) {
		case NotifyMsg::FocusIn: InnerBorderFrame::SetBorder(Border(2.0f, Color::Red)); break;
		case NotifyMsg::FocusOut: InnerBorderFrame::SetBorder(Border(1.0f, Color::Black)); break;
		}
	}
};

template<class T>
HighlightFocus(T) -> HighlightFocus<InnerBorderFrame<extract_width_type<T>, extract_height_type<T>>>;


struct TextBoxStyle : TextBox::Style {
	TextBoxStyle() {
		paragraph.line_spacing(200pct);
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
		new MainFrame(
			L"ListLayoutTest",
			new ScrollFrame(
				new ListLayout<Vertical>(
					20,
					new HighlightFocus(
						new ClipFrame<Assigned, Auto, Left>(
							new TextBox(TextBoxStyle(), L"ListLayoutTest")
						)
					),
					new HighlightFocus(
						new ClipFrame<Assigned, Auto, Left>(
							new EditBox(EditBoxStyle1(), L"EditBox")
						)
					),
					new HighlightFocus(
						new HitThroughMargin<ClipFrame<Assigned, Auto, Right>>(
							new EditBox(EditBoxStyle2(), L"EditBox2")
						)
					)
				)
			)
		)
	);
	global.MessageLoop();
}