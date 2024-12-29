#include "WndDesign/window/Global.h"
#include "WndDesign/widget/TitleBarFrame.h"
#include "WndDesign/layout/FlowLayout.h"
#include "WndDesign/control/Button.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/frame/BorderFrame.h"
#include "WndDesign/frame/PaddingFrame.h"
#include "WndDesign/frame/MaxFrame.h"
#include "WndDesign/frame/ScrollFrame.h"
#include "WndDesign/wrapper/Background.h"


using namespace WndDesign;


struct MainFrameStyle : TitleBarFrame::Style {
	MainFrameStyle() {
		width.normal(560px);
		height.normal(180px);
		title.text.assign(L"FlowLayoutTest");
	}
};


class ScrollBox : public ScrollFrame<Vertical> {
public:
	using ScrollFrame::ScrollFrame;
private:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		if (msg.type == MouseMsg::WheelVertical) {
			return this;
		}
		return ScrollFrame::HitTest(msg);
	}
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		Scroll(-(float)msg.wheel_delta);
	}
};


class MyFlowLayout : public SolidColorBackground<FlowLayout> {
public:
	MyFlowLayout() : Base(25, 10, 5) {}
private:
	struct EditBoxStyle : public EditBox::Style {
		EditBoxStyle() {
			paragraph.line_spacing(90pct);
			font.family(L"Segoe UI").size(17);
		}
	};
	std::wstring NextName() {
		static const wchar* names[27] = {
			L"Alice", L"Bob", L"Carol", L"Dave", L"Eve", L"Francis", L"Grace", L"Hans", L"Isabella",
			L"Jason", L"Kate", L"Louis", L"Margaret", L"Nathan", L"Olivia", L"Paul", L"Queen", L"Richard",
			L"Susan", L"Thomas", L"Uma", L"Vivian", L"Winnie", L"Xander", L"Yasmine", L"Zach", L"You Win"
		};
		static uint next = 0;
		return names[next >= 26 ? next : next++];
	}
private:
	virtual ref_ptr<WndObject> HitTest(MouseMsg& msg) override {
		ref_ptr<WndObject> wnd = Base::HitTest(msg);
		return wnd == nullptr ? this : wnd;
	}
	virtual void OnMouseMsg(MouseMsg msg) override {
		if (msg.type == MouseMsg::LeftDown) {
			AppendChild(
				new BorderFrame(
					Border(3px, 12px, Color::CadetBlue),
					new PaddingFrame(
						Padding(6, 0),
						new ClipFrame<Auto, Assigned, Bottom> (
							new MaxFrame(
								size_max,
								new EditBox(EditBoxStyle(), NextName())
							)
						)
					)
				)
			);
		}
	}
};


int main() {
	global.AddWnd(
		new TitleBarFrame(
			MainFrameStyle(),
			new ScrollBox(
				new PaddingFrame(
					Padding(10),
					new MyFlowLayout
				)
			)
		)
	);
	global.MessageLoop();
}