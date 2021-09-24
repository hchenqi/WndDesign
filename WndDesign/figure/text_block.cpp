#include "text_block.h"

#include "../system/d2d_api.h"
#include "../system/dwrite_api.h"
#include "../system/directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


TextBlock::~TextBlock() {
	SafeRelease(&layout);
}

void TextBlock::SetText(const TextBlockStyle& style, const std::wstring& text) {
	SafeRelease(&layout);

	ComPtr<IDWriteTextFormat> format;
	hr << GetDWriteFactory().CreateTextFormat(
		style.font._family.c_str(),
		NULL,
		static_cast<DWRITE_FONT_WEIGHT>(style.font._weight),
		static_cast<DWRITE_FONT_STYLE>(style.font._style),
		static_cast<DWRITE_FONT_STRETCH>(style.font._stretch),
		static_cast<FLOAT>(style.font._size),
		style.font._locale.c_str(),
		&format
	);

	ComPtr<IDWriteTextLayout> layout;
	hr << GetDWriteFactory().CreateTextLayout(
		text.c_str(), static_cast<UINT>(text.length()),
		format.Get(),
		0, 0,
		&layout
	);

	layout->SetTextAlignment(static_cast<DWRITE_TEXT_ALIGNMENT>(style.paragraph._text_align));
	layout->SetParagraphAlignment(static_cast<DWRITE_PARAGRAPH_ALIGNMENT>(style.paragraph._paragraph_align));
	layout->SetFlowDirection(static_cast<DWRITE_FLOW_DIRECTION>(style.paragraph._flow_direction));
	layout->SetReadingDirection(static_cast<DWRITE_READING_DIRECTION>(style.paragraph._read_direction));
	layout->SetWordWrapping(static_cast<DWRITE_WORD_WRAPPING>(style.paragraph._word_wrap));
	ValueTag line_height = style.paragraph._line_height;
	if (line_height.IsPixel()) {
		layout->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_UNIFORM, static_cast<FLOAT>(line_height.AsUnsigned()), 0.7F * line_height.AsUnsigned());
	} else if (line_height.IsPercent()) {
		layout->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_PROPORTIONAL, line_height.AsUnsigned() / 100.0F, line_height.AsUnsigned() / 110.0F);
	} else {
		layout->SetLineSpacing(DWRITE_LINE_SPACING_METHOD_DEFAULT, 0.0F, 0.0F);  // The last two parameters are ignored.
	}
	ValueTag tab_size = style.paragraph._tab_size;
	if (tab_size.IsPixel()) {
		layout->SetIncrementalTabStop(static_cast<FLOAT>(tab_size.AsUnsigned()));
	} else if (tab_size.IsPercent()) {
		layout->SetIncrementalTabStop(static_cast<FLOAT>(tab_size.AsUnsigned()) * style.font._size / 100.0F);
	}

	this->layout = static_cast<TextLayout*>(layout.Detach());
}

Size TextBlock::UpdateSizeRef(Size size_ref) {
	layout->SetMaxWidth(static_cast<FLOAT>(size_ref.width));
	layout->SetMaxHeight(static_cast<FLOAT>(size_ref.height));
	DWRITE_TEXT_METRICS metrics; layout->GetMetrics(&metrics);
	return Size(
		static_cast<uint>(ceil(metrics.widthIncludingTrailingWhitespace)),
		static_cast<uint>(ceil(metrics.height))
	);
}


inline TextBlockHitTestInfo HitTestMetricsToInfo(const DWRITE_HIT_TEST_METRICS& metrics, bool is_inside, bool is_trailing_hit) {
	Point left_top = Point((int)roundf(metrics.left), (int)roundf(metrics.top));
	Point right_bottom = Point((int)roundf(metrics.left + metrics.width), (int)roundf(metrics.top + metrics.height));
	return TextBlockHitTestInfo{
		metrics.textPosition,
		metrics.length,
		is_inside,
		is_trailing_hit,
		Rect(left_top, Size(uint(right_bottom.x - left_top.x), uint(right_bottom.y - left_top.y)))
	};
}

TextBlockHitTestInfo TextBlock::HitTestPoint(Point point) const {
	BOOL isTrailingHit; BOOL isInside; DWRITE_HIT_TEST_METRICS metrics;
	layout->HitTestPoint(static_cast<FLOAT>(point.x), static_cast<FLOAT>(point.y), &isTrailingHit, &isInside, &metrics);
	return HitTestMetricsToInfo(metrics, (bool)isInside, (bool)isTrailingHit);
}

TextBlockHitTestInfo TextBlock::HitTestTextPosition(uint text_position) const {
	FLOAT x, y; DWRITE_HIT_TEST_METRICS metrics;
	layout->HitTestTextPosition(text_position, false, &x, &y, &metrics);
	return HitTestMetricsToInfo(metrics, true, false);
}

std::vector<TextBlockHitTestInfo> TextBlock::HitTestTextRange(uint text_position, uint text_length) const {
	UINT32 line_cnt; layout->GetLineMetrics((DWRITE_LINE_METRICS1*)nullptr, 0, &line_cnt);
	UINT32 actual_size = line_cnt;
	std::vector<DWRITE_HIT_TEST_METRICS> metrics;
	do {
		metrics.resize(actual_size);
		layout->HitTestTextRange(text_position, text_length, 0, 0, metrics.data(), static_cast<UINT32>(metrics.size()), &actual_size);
	} while (actual_size > metrics.size());
	metrics.resize(actual_size);
	std::vector<TextBlockHitTestInfo> geometry_regions(metrics.size());
	for (size_t i = 0; i < geometry_regions.size(); ++i) {
		geometry_regions[i] = HitTestMetricsToInfo(metrics[i], true, false);
		if (geometry_regions[i].geometry_region.size.width < 5) { 
			geometry_regions[i].geometry_region.size.width = 5; 
		}
	}
	return geometry_regions;
}


void TextBlockFigure::DrawOn(RenderTarget& target, Point point) const {
	target.DrawTextLayout(
		Point2POINT(point),
		text_block.layout,
		&GetD2DSolidColorBrush(font_color),
		D2D1_DRAW_TEXT_OPTIONS_CLIP | D2D1_DRAW_TEXT_OPTIONS_ENABLE_COLOR_FONT
	);
}


END_NAMESPACE(WndDesign)