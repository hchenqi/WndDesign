#pragma once

#include "figure.h"
#include "../common/uncopyable.h"
#include "../style/text_block_style.h"

#include <vector>


BEGIN_NAMESPACE(WndDesign)

struct TextLayout;


class TextBlock : Uncopyable {
private:
	friend struct TextBlockFigure;
public:
	TextBlock(const TextBlockStyle& style, const std::wstring& text) : layout(nullptr) { SetText(style, text); }
	~TextBlock();
protected:
	alloc_ptr<TextLayout> layout;
public:
	void SetText(const TextBlockStyle& style, const std::wstring& text);
	void UpdateSizeRef(Size size_ref);
	Size GetSize() const;
public:
	struct HitTestInfo {
		uint text_position = 0;
		uint text_length = 0;
		Rect geometry_region = region_empty;
	};
public:
	HitTestInfo HitTestPoint(Point point) const;
	HitTestInfo HitTestTextPosition(size_t text_position) const;
	std::vector<HitTestInfo> HitTestTextRange(size_t text_position, size_t text_length) const;
};


struct TextBlockFigure : Figure {
	const TextBlock& text_block;
	Color font_color;

	TextBlockFigure(const TextBlock& text_block, Color font_color) : text_block(text_block), font_color(font_color) {}
	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


END_NAMESPACE(WndDesign)