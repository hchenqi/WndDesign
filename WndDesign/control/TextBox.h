#pragma once

#include "../window/wnd_traits.h"
#include "../figure/text_block.h"


BEGIN_NAMESPACE(WndDesign)


class TextBox : public WndType<Relative, Auto> {
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
	void TextUpdated() {
		text_block.SetText(style, text);
		SizeUpdated(Size(width_ref, text_block.UpdateSizeRef(Size(width_ref, length_max)).height));
		Redraw(region_infinite);
	}
protected:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		width_ref = size_ref.width;
		return Size(width_ref, text_block.UpdateSizeRef(Size(width_ref, length_max)).height);
	}
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const {
		figure_queue.add(point_zero, new TextBlockFigure(text_block, style.font._color));
	}
};


END_NAMESPACE(WndDesign)