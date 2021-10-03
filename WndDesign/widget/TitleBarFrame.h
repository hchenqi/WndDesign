#pragma once

#include "../frame/DesktopFrame.h"
#include "../frame/ClipFrame.h"
#include "../frame/MaxFrame.h"
#include "../layout/SplitLayout.h"
#include "../layout/BarLayout.h"
#include "../layout/ListLayout.h"
#include "../control/TextBox.h"
#include "../control/Button.h"
#include "../message/mouse_tracker.h"
#include "../system/win32_aero_snap.h"


BEGIN_NAMESPACE(WndDesign)


class TitleBarFrame : public DesktopFrame {
public:
	using child_ptr = DesktopFrame::child_ptr;
	using child_ptr_menu = WndDesign::child_ptr<Auto, Assigned>;

private:
	struct TitleBarFrameStyle {
		struct TitleBarStyle {
		public:
			uint _height = 30;
			uint _max_title_length = 300;
			Color _background = Color::DarkGray;
		public:
			constexpr TitleBarStyle& height(uint height) { _height = height; return *this; }
			constexpr TitleBarStyle& maxt_title_length(uint maxt_title_length) { _max_title_length = maxt_title_length; return *this; }
			constexpr TitleBarStyle& background(Color background) { _background = background; return *this; }
		}title_bar;

		TextBox::Style title_format;
	};
public:
	struct Style : DesktopFrame::Style, TitleBarFrameStyle {};

public:
	TitleBarFrame(Style style, child_ptr child, child_ptr_menu menu = new Placeholder<Auto, Assigned>(0)) : DesktopFrame{
		style,
		new SplitLayout<Vertical, First>{
			new TitleBar(*this, style, std::move(menu)),
			std::move(child)
		}
	} {
	}

private:
	ref_ptr<TextBox> title = nullptr;
public:
	void SetTitle(std::wstring str) { title->SetText(str); }

private:
	bool IsMaximized() const { return status == Status::Maximized; }
	void MaximizeOrRestore() { if (status == Status::Normal) { Maximize(); } else if (status == Status::Maximized) { Restore(); } }

private:
	class TitleBar : public BarLayout<Horizontal> {
	public:
		TitleBar(TitleBarFrame& frame, const Style& style, child_ptr_menu menu) : BarLayout(
			style.title_bar._height,
			std::move(menu),
			new ListLayout<Horizontal, 3>{
				0,
				new MinimizeButton(frame, style.title_bar._background),
				new MaximizeButton(frame, style.title_bar._background),
				new CloseButton(frame, style.title_bar._background)
			},
			new ClipFrame<Auto, Assigned>{
				new MaxFrame{
					style.title_bar._max_title_length,
					new Title(frame, style.title_format, style.title)
				}
			}
		), frame(frame), background(style.title_bar._background) {
		}
	private:
		TitleBarFrame& frame;
	private:
		Color background;
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
			case MouseTrackMsg::LeftDoubleClick: frame.MaximizeOrRestore(); break;
			}
		}
	};

	class Title : public TextBox {
	public:
		Title(TitleBarFrame& frame, TextBox::Style style, std::wstring text) : TextBox(style, text) { frame.title = this; }
	};

	class ButtonBase : public Button<Auto, Assigned> {
	public:
		ButtonBase(TitleBarFrame& frame, Color background) : Button<Auto, Assigned>(50), frame(frame) {
			this->background = this->background_normal = background;
		}
	protected:
		TitleBarFrame& frame;
	};

	class MinimizeButton : public ButtonBase {
	public:
		using ButtonBase::ButtonBase;
	private:
		virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
			ButtonBase::OnDraw(figure_queue, draw_region);
			figure_queue.add(Point(20, 15), new Line(Vector(10, 0), 1.0, Color::White));
		}
	private:
		virtual void OnClick() override { frame.Minimize(); }
	};

	class MaximizeButton : public ButtonBase {
	public:
		using ButtonBase::ButtonBase;
	private:
		virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
			ButtonBase::OnDraw(figure_queue, draw_region);
			if (frame.IsMaximized()) {
				figure_queue.add(Point(22, 10), new Rectangle(Size(8, 8), 1.0, Color::White));
				figure_queue.add(Point(20, 12), new Rectangle(Size(8, 8), background, 1.0, Color::White));
			} else {
				figure_queue.add(Point(20, 10), new Rectangle(Size(10, 10), 1.0, Color::White));
			}
		}
	private:
		virtual void OnClick() override { frame.MaximizeOrRestore(); }
	};

	class CloseButton : public ButtonBase {
	public:
		using ButtonBase::ButtonBase;
	private:
		virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
			ButtonBase::OnDraw(figure_queue, draw_region);
			figure_queue.add(Point(20, 10), new Line(Vector(10, 10), 1.0, Color::White));
			figure_queue.add(Point(20, 20), new Line(Vector(10, -10), 1.0, Color::White));
		}
	private:
		virtual void OnClick() override { frame.Destroy(); }
	};
};


END_NAMESPACE(WndDesign)