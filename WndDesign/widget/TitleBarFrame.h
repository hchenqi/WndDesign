#pragma once

#include "../frame/DesktopFrame.h"
#include "../frame/ClipFrame.h"
#include "../frame/MaxFrame.h"
#include "../layout/SplitLayout.h"
#include "../layout/BarLayout.h"
#include "../layout/ListLayout.h"
#include "../control/TextBox.h"
#include "../control/Button.h"
#include "../wrapper/Background.h"
#include "../message/mouse_tracker.h"
#include "../system/win32_aero_snap.h"


BEGIN_NAMESPACE(WndDesign)


class TitleBarFrame : public DesktopFrame {
public:
	using child_ptr = DesktopFrame::child_ptr;
	using child_ptr_menu = WndDesign::child_ptr<Auto, Assigned>;

public:
	struct Style : DesktopFrame::Style {
		struct TitleBarStyle {
		public:
			float _height = 30.0f;
			float _max_title_length = 300.0f;
			Color _background = Color::DarkGray;
		public:
			constexpr TitleBarStyle& height(float height) { _height = height; return *this; }
			constexpr TitleBarStyle& maxt_title_length(float maxt_title_length) { _max_title_length = maxt_title_length; return *this; }
			constexpr TitleBarStyle& background(Color background) { _background = background; return *this; }
		}title_bar;

		TextBox::Style title_format;
	};

public:
	TitleBarFrame(Style style, child_ptr child, child_ptr_menu menu = new Placeholder<Auto, Assigned>(0.0f)) : DesktopFrame{
		style,
		new SplitLayout<Vertical, First>{
			new TitleBar(*this, style, std::move(menu)),
			std::move(child)
		}
	}, title(title) {
	}

private:
	ref_ptr<TextBox> title;
public:
	void SetTitle(std::wstring str) { title->SetText(str); DesktopFrame::SetTitle(str); }

protected:
	bool IsMaximized() const { return status == Status::Maximized; }
	void MaximizeOrRestore() { if (status == Status::Normal) { Maximize(); } else if (status == Status::Maximized) { Restore(); } }

protected:
	class TitleBar : public Decorate<BarLayout, SolidColorBackground> {
	public:
		using Style = TitleBarFrame::Style;
	public:
		TitleBar(TitleBarFrame& frame, const Style& style, child_ptr_menu menu) : Base(
			style.title_bar._height,
			std::move(menu),
			new ListLayout<Horizontal>{
				0.0f,
				new MinimizeButton(frame, style.title_bar._background),
				new MaximizeButton(frame, style.title_bar._background),
				new CloseButton(frame, style.title_bar._background)
			},
			new ClipFrame<Auto, Assigned>{
				new MaxFrame{
					style.title_bar._max_title_length,
					new Title(frame, *this, style)
				}
			}
		), frame(frame) {
			background = style.title_bar._background;
		}
	private:
		TitleBarFrame& frame;
	private:
		MouseTracker mouse_tracker;
	private:
		virtual void OnMouseMsg(MouseMsg msg) override {
			switch (mouse_tracker.Track(msg)) {
			case MouseTrackMsg::LeftDown: AeroSnapDraggingEffect(*this); break;
			case MouseTrackMsg::LeftDoubleClick: frame.MaximizeOrRestore(); break;
			}
		}

	private:
		class Title : public Decorate<TextBox, WndDesign::SolidColorBackground> {
		public:
			Title(TitleBarFrame& frame, TitleBar& title_bar, const TitleBarFrame::Style& style) :
				Base(style.title_format, style.title), title_bar(title_bar) {
				frame.title = this;
				background = style.title_bar._background;
			}
		private:
			TitleBar& title_bar;
		private:
			virtual void OnMouseMsg(MouseMsg msg) { title_bar.OnMouseMsg(msg); }
		};

		class ButtonBase : public Button<Auto, Assigned> {
		public:
			ButtonBase(TitleBarFrame& frame, Color background) : Button<Auto, Assigned>(50.0f), frame(frame) {
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
				figure_queue.add(Point(20.0f, 15.0f), new Rectangle(Size(10.0f, 1.0f), 1.0f, Color::White));
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
					figure_queue.add(Point(22.0f, 10.0f), new Rectangle(Size(8.0f, 8.0f), 1.0f, Color::White));
					figure_queue.add(Point(20.0f, 12.0f), new Rectangle(Size(8.0f, 8.0f), background, 1.0f, Color::White));
				} else {
					figure_queue.add(Point(20.0f, 10.0f), new Rectangle(Size(10.0f, 10.0f), 1.0f, Color::White));
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
				figure_queue.add(Point(20.0f, 10.0f), new Line(Vector(10.0f, 10.0f), 1.0f, Color::White));
				figure_queue.add(Point(20.0f, 20.0f), new Line(Vector(10.0f, -10.0f), 1.0f, Color::White));
			}
		private:
			virtual void OnClick() override { frame.Destroy(); }
		};
	};
};


END_NAMESPACE(WndDesign)