#pragma once

#include "../window/wnd_traits.h"
#include "../figure/text_block.h"
#include "../system/cursor.h"


BEGIN_NAMESPACE(WndDesign)


class TextBox : public WndType<Assigned, Auto> {
public:
	using Style = TextBlockStyle;
public:
	TextBox(Style style, std::wstring text) : style(style), text(text) {}
	~TextBox() {}
private:
	Style style;
	uint width_ref = 0;
protected:
	std::wstring text;
	TextBlock text_block = TextBlock(style, text);
protected:
	Size UpdateSize() {
		return Size(width_ref, text_block.UpdateSizeRef(Size(width_ref, length_max)).height);
	}
	void TextUpdated() {
		text_block.SetText(style, text);
		SizeUpdated(UpdateSize());
		Redraw(region_infinite);
	}
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override {
		width_ref = size_ref.width; return UpdateSize();
	}
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(point_zero, new TextBlockFigure(text_block, style.font._color));
	}
protected:
	virtual void OnNotifyMsg(NotifyMsg msg) override {
		switch (msg) {
		case NotifyMsg::MouseEnter: SetCursor(Cursor::Default); break;
		}
	}
};


END_NAMESPACE(WndDesign)