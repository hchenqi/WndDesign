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
	alloc_ptr<TextLayout> layout;
public:
	TextBlock(const TextBlockStyle& style, const std::wstring& text) : layout(nullptr) { SetText(style, text); }
	~TextBlock();
public:
	void SetText(const TextBlockStyle& style, const std::wstring& text);
	void UpdateSizeRef(Size size_ref);
	Size GetSize() const;
public:
	TextBlockHitTestInfo HitTestPoint(Point point) const;
	TextBlockHitTestInfo HitTestTextPosition(uint text_position) const;
	std::vector<TextBlockHitTestInfo> HitTestTextRange(uint text_position, uint text_length) const;
};


struct TextBlockFigure : Figure {
	const TextBlock& text_block;
	Color font_color;

	TextBlockFigure(const TextBlock& text_block, Color font_color) : text_block(text_block), font_color(font_color) {}
	virtual void DrawOn(RenderTarget& target, Point point) const override;
};


END_NAMESPACE(WndDesign)