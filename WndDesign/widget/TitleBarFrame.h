#pragma once

#include "../frame/DesktopFrame.h"
#include "../frame/BorderFrame.h"
#include "../frame/ClipFrame.h"
#include "../frame/MinMaxFrame.h"
#include "../layout/SplitLayout.h"
#include "../layout/BarLayout.h"
#include "../layout/ListLayout.h"
#include "../control/TextBox.h"
#include "../control/Button.h"
#include "../wrapper/Background.h"
#include "../style/length_style_helper.h"
#include "../message/mouse_tracker.h"
#include "../system/cursor.h"
#include "../system/win32_aero_snap.h"


BEGIN_NAMESPACE(WndDesign)


class TitleBarFrame : public DesktopFrame {
public:
	using child_ptr = WndDesign::child_ptr<Assigned, Assigned>;
	using child_ptr_menu = WndDesign::child_ptr<Auto, Assigned>;

public:
	struct TitleBarStyle {
	public:
		float _height = 30.0f;
		float _max_title_length = 300.0f;
		Color _background = Color::DarkGray;
	public:
		constexpr TitleBarStyle& height(float height) { _height = height; return *this; }
		constexpr TitleBarStyle& max_title_length(float max_title_length) { _max_title_length = max_title_length; return *this; }
		constexpr TitleBarStyle& background(Color background) { _background = background; return *this; }
	};

	struct Style {
		LengthStyle width;
		LengthStyle height;
		PositionStyle position;
		Border border;
		TitleBarStyle title_bar;
		std::wstring title;
		TextBox::Style title_format;
		Color background_color;

		Style() {
			width.normal(800px).min(200px).max(100pct);
			height.normal(500px).min(200px).max(100pct);
			position.setHorizontalCenter().setVerticalCenter();
			border.width(4).radius(4).color(Color::CadetBlue);
			title_bar.background(Color::CadetBlue);
			title.assign(L"TitleBarFrame");
			title_format.font.size(18px).color(Color::White);
			background_color = Color::White;
		}
	};

public:
	TitleBarFrame(Style style, child_ptr child, child_ptr_menu menu = new Placeholder<Auto, Assigned>(0.0f)) : DesktopFrame{
		style.title,
		new ResizeBorder{
			style.border,
			new SplitLayoutVertical{
				new TitleBar(*this, style, std::move(menu)),
				std::move(child)
			}
		}
	}, style(style), title(title) {
	}

	// style
private:
	Style style;
private:
	ref_ptr<TextBox> title;
public:
	void SetTitle(std::wstring str) { title->Assign(str); DesktopFrame::SetTitle(str); }

	// layout
private:
	virtual std::pair<Size, Size> CalculateMinMaxSize(Size size_ref) override {
		return LengthStyleHelper::CalculateMinMaxSize(style.width, style.height, size_ref);
	}
	virtual Rect OnDesktopFrameSizeRefUpdate(Size size_ref) override {
		Rect region = LengthStyleHelper::CalculateRegion(style.width, style.height, style.position, size_ref);
		UpdateChildSizeRef(child, region.size);
		return region;
	}

	// paint
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		Rect child_region = GetBorder().GetChildRegion();
		figure_queue.add(child_region.point, new Rectangle(child_region.size, style.background_color));
		DrawChild(child, point_zero, figure_queue, draw_region);
	}

	// state
private:
	bool IsMaximized() { return GetState() == State::Maximized; }
	void MaximizeOrRestore() { if (GetState() == State::Normal) { Maximize(); } else if (GetState() == State::Maximized) { Restore(); } }
private:
	virtual void OnStateChange(State state) {
		switch (state) {
		case State::Normal: GetBorder().Restore(); break;
		case State::Maximized: GetBorder().Hide(); break;
		}
	}

	// child
private:
	class ResizeBorder : public BorderFrame<Assigned, Assigned> {
	public:
		ResizeBorder(Border border, child_ptr child) : BorderFrame<Assigned, Assigned>(border, std::move(child)), border_copy(border) {
			cursor = Cursor::Hide;
		}
	public:
		BorderFrame::GetChildRegion;
	protected:
		Border border_copy;
	public:
		void Hide() { border = {}; }
		void Restore() { border = border_copy; }
	protected:
		virtual void OnMouseMsg(MouseMsg msg) override {
			if (msg.type == MouseMsg::Move || msg.type == MouseMsg::LeftDown) {
				BorderPosition border_position = HitTestBorderPosition(size, border._width + border._radius, msg.point);
				if (msg.type == MouseMsg::Move) {
					SetCursor(GetBorderPositionCursor(border_position));
				}
				else {
					AeroSnapBorderResizingEffect(*this, border_position);
				}
			}
		}
	};
private:
	ResizeBorder& GetBorder() { return static_cast<ResizeBorder&>(*child); }

private:
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
					Size(style.title_bar._max_title_length, length_max),
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