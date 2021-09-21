#pragma once

#include "../window/wnd_traits.h"
#include "../figure/text_block.h"


BEGIN_NAMESPACE(WndDesign)


class TextBox : public WndType<Relative, Auto> {
public:
	TextBox(TextBlockStyle style, std::wstring text) : style(style), text(text) {}
	~TextBox() {}
private:
	TextBlockStyle style;
	std::wstring text;
	TextBlock text_block = TextBlock(style, text);
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override {
		return text_block.UpdateSizeRef(Size(size_ref.width, length_max));
	}
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const {
		figure_queue.add(point_zero, new TextBlockFigure(text_block, style.font._color));
	}
};


END_NAMESPACE(WndDesign)