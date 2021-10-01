#include "WndDesign/window/Global.h"
#include "WndDesign/frame/DesktopFrame.h"
#include "WndDesign/frame/CenterFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/frame/MaxFrame.h"
#include "WndDesign/layout/SplitLayout.h"
#include "WndDesign/layout/BarLayout.h"
#include "WndDesign/layout/ListLayout.h"
#include "WndDesign/control/Button.h"
#include "WndDesign/control/TextBox.h"
#include "WndDesign/control/ImageBox.h"
#include "WndDesign/system/win32_aero_snap.h"
#include "WndDesign/message/mouse_tracker.h"


using namespace WndDesign;


class MainFrame : public DesktopFrame {
private:
	struct Style : DesktopFrameStyle {
		Style() {
			width.normal(800px).max(100pct);
			height.normal(500px).max(100pct);
			position.setHorizontalCenter().setVerticalCenter();
			border.width(3).color(Color::Violet);
			title.assign(L"BarLayoutTest");
		}
	};
public:
	MainFrame();
public:
	DesktopFrame::Minimize;
	DesktopFrame::Destroy;
public:
	bool IsMaximized() const { return status == Status::Maximized; }
	void MaximizeOrRestore() { if (status == Status::Normal) { Maximize(); } else if (status == Status::Maximized) { Restore(); } }
};


class Menubar : public BarLayout<Horizontal> {
public:
	Menubar(MainFrame& main_frame) : BarLayout(
		30,
		new Placeholder<Auto, Assigned>(0),
		new ListLayout<Horizontal, 3>{
			0,
			new MinimizeButton(main_frame),
			new MaximizeButton(main_frame),
			new CloseButton(main_frame)
		},
		new ClipFrame<Auto, Assigned>{
			new MaxFrame{
				300,
				new TextBox(TextBox::Style(), L"BarLayoutTest")
			}
		}
	), main_frame(main_frame) {
	}
private:
	MainFrame& main_frame;
private:
	static constexpr Color background = Color::SpringGreen;
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(draw_region.point, new Rectangle(draw_region.size, background));
		BarLayout::OnDraw(figure_queue, draw_region);
	}
private:
	MouseTracker mouse_tracker;
private:
	virtual void OnMouseMsg(MouseMsg msg) override {
		switch (mouse_tracker.Track(msg)) {
		case MouseTrackMsg::LeftDown: AeroSnapDraggingEffect(*this, msg.point); break;
		case MouseTrackMsg::LeftDoubleClick: main_frame.MaximizeOrRestore(); break;
		}
	}
private:
	class Button : public WndDesign::Button<Auto, Assigned> {
	public:
		Button(MainFrame& main_frame) : WndDesign::Button<Auto, Assigned>(50), main_frame(main_frame) {}
	public:
		MainFrame& main_frame;
	private:
		static constexpr Color background_normal = Color::DarkGray;
		static constexpr Color background_hover = Color::Gray;
		static constexpr Color background_press = Color::DimGray;
	protected:
		Color background = background_normal;
	protected:
		void SetBackgroundColor(Color color) { background = color; Redraw(region_infinite); }
	protected:
		virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
			figure_queue.add(draw_region.point, new Rectangle(draw_region.size, background));
		}
	private:
		virtual void OnHover() override { SetBackgroundColor(background_hover); }
		virtual void OnPress() override { SetBackgroundColor(background_press); }
		virtual void OnLeave() override { SetBackgroundColor(background_normal); }
	};
	class MinimizeButton : public Button {
	public:
		using Button::Button;
	private:
		virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
			Button::OnDraw(figure_queue, draw_region);
			figure_queue.add(Point(20, 15), new Line(Vector(10, 0), 1.0, Color::White));
		}
	private:
		virtual void OnClick() override { main_frame.Minimize(); }
	};
	class MaximizeButton : public Button {
	public:
		using Button::Button;
	private:
		virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
			Button::OnDraw(figure_queue, draw_region);
			if (main_frame.IsMaximized()) {
				figure_queue.add(Point(22, 10), new Rectangle(Size(8, 8), 1.0, Color::White));
				figure_queue.add(Point(20, 12), new Rectangle(Size(8, 8), background, 1.0, Color::White));
			} else {
				figure_queue.add(Point(20, 10), new Rectangle(Size(10, 10), 1.0, Color::White));
			}
		}
	private:
		virtual void OnClick() override { main_frame.MaximizeOrRestore(); }
	};
	class CloseButton : public Button {
	public:
		using Button::Button;
	private:
		virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
			Button::OnDraw(figure_queue, draw_region);
			figure_queue.add(Point(20, 10), new Line(Vector(10, 10), 1.0, Color::White));
			figure_queue.add(Point(20, 20), new Line(Vector(10, -10), 1.0, Color::White));
		}
	private:
		virtual void OnClick() override { main_frame.Destroy(); }
	};
};


MainFrame::MainFrame() : DesktopFrame(
	Style(),
	new SplitLayout<Vertical, First>{
		new Menubar(*this),
		new CenterFrame<Assigned, Assigned>{
			new ImageBox(L"R:\\1.jpg")
		}
	}
) {
}


int main() {
	global.AddWnd(new MainFrame);
	global.MessageLoop();
}