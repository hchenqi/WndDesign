#pragma once

#include "../window/wnd_traits.h"
#include "../figure/text_block.h"


BEGIN_NAMESPACE(WndDesign)


class TextBox : public WndType<Relative, Auto> {
public:
	using Style = TextBlockStyle;
public:
	TextBox(Style style, std::wstring text) : style(style), text(text), text_block(style, text) {}
	~TextBox() {}

	// style
private:
	Style style;
	float width_ref = 0.0f;

	// text
protected:
	std::wstring text;
	TextBlock text_block;
private:
	void UpdateSize() { text_block.UpdateSizeRef(Size(width_ref, length_max)); }
	void TextUpdated() { text_block.SetText(style, text); UpdateSize(); redraw_region = region_infinite; SizeUpdated(); Redraw(); }
public:
	void SetText(std::wstring str) { text.assign(std::move(str)); TextUpdated(); }
	void InsertText(uint pos, wchar ch) { text.insert(pos, 1, ch); TextUpdated(); }
	void InsertText(uint pos, std::wstring str) { text.insert(pos, str); TextUpdated(); }
	void ReplaceText(uint begin, uint length, wchar ch) { text.replace(begin, length, 1, ch); TextUpdated(); }
	void ReplaceText(uint begin, uint length, std::wstring str) { text.replace(begin, length, str); TextUpdated(); }
	void DeleteText(uint begin, uint length) { text.erase(begin, length); TextUpdated(); }

	// layout
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { width_ref = size_ref.width; UpdateSize(); }
	virtual Size GetSize() override { return text_block.GetSize(); }

	// paint
protected:
	Rect redraw_region = region_empty;
protected:
	virtual Rect GetRedrawRegion() { return redraw_region; }
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(point_zero, new TextBlockFigure(text_block, style.font._color));
		redraw_region = region_empty;
	}
};


END_NAMESPACE(WndDesign)