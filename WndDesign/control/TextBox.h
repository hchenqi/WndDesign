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

	// text
protected:
	Style style;
	std::wstring text;
	TextBlock text_block;
protected:
	void TextUpdated() { text_block.SetText(style, text); SizeUpdated(UpdateLayout()); Redraw(region_infinite); }
public:
	void SetText(std::wstring str) { text.assign(std::move(str)); TextUpdated(); }
	void InsertText(size_t pos, wchar ch) { text.insert(pos, 1, ch); TextUpdated(); }
	void InsertText(size_t pos, std::wstring str) { text.insert(pos, str); TextUpdated(); }
	void ReplaceText(size_t begin, size_t length, wchar ch) { text.replace(begin, length, 1, ch); TextUpdated(); }
	void ReplaceText(size_t begin, size_t length, std::wstring str) { text.replace(begin, length, str); TextUpdated(); }
	void DeleteText(size_t begin, size_t length) { text.erase(begin, length); TextUpdated(); }
public:
	void ModifyText(std::function<void(std::wstring&)> func) { func(text); TextUpdated(); }

	// layout
protected:
	float width_ref = 0.0f;
protected:
	Size UpdateLayout() { text_block.UpdateSizeRef(Size(width_ref, length_max)); return text_block.GetSize(); }
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override { width_ref = size_ref.width; return UpdateLayout(); }

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(point_zero, new TextBlockFigure(text_block, style.font._color));
	}
};


END_NAMESPACE(WndDesign)