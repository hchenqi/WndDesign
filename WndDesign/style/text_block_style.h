#pragma once

#include "value_tag.h"
#include "../figure/color.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)


enum class TextAlign : uchar {
	Leading,
	Trailing,
	Center,
	Justified,
};

enum class ParagraphAlign : uchar {
	Top,
	Bottom,
	Center,
};

enum class FlowDirection : uchar {
	TopToBottom = 0,
	BottomToTop = 1,
	LeftToRight = 2,
	RightToLeft = 3,
};

enum class ReadDirection : uchar {
	LeftToRight = 0,
	RightToLeft = 1,
	TopToBottom = 2,
	BottomToTop = 3,
};

enum class WordWrap : uchar {
	Wrap = 0,
	NoWrap = 1,
	EmergencyBreak = 2,
	WholeWord = 3,
	Character = 4,
};

enum class FontWeight : ushort {
	Thin = 100,
	ExtraLight = 200,
	UltraLight = 200,
	Light = 300,
	SemiLight = 350,
	Normal = 400,
	Regular = 400,
	Medium = 500,
	DemiBold = 600,
	SemiBold = 600,
	Bold = 700,
	ExtraBold = 800,
	UltraBold = 800,
	Black = 900,
	Heavy = 900,
	ExtraBlack = 950,
	UltraBlack = 950,
};

enum class FontStretch : uchar {
	Undefined = 0,
	Ultra_condensed = 1,
	Extra_condensed = 2,
	Condensed = 3,
	Semi_condensed = 4,
	Normal = 5,
	Medium = 5,
	Semi_expanded = 6,
	Expanded = 7,
	Extra_expanded = 8,
	Ultra_expanded = 9
};

enum class FontStyle : uchar {
	Normal,
	Oblique,
	Italic
};


struct TextBlockStyle {
	struct ParagraphFormat {
	public:
		TextAlign _text_align = TextAlign::Leading;
		ParagraphAlign _paragraph_align = ParagraphAlign::Top;
		FlowDirection _flow_direction = FlowDirection::TopToBottom;
		ReadDirection _read_direction = ReadDirection::LeftToRight;
		WordWrap _word_wrap = WordWrap::Wrap;
		ValueTag _line_height = 120pct;  // relative to font size
		ValueTag _tab_size = 200pct;
	public:
		constexpr ParagraphFormat& text_align(TextAlign text_align) { _text_align = text_align; return *this; }
		constexpr ParagraphFormat& paragraph_align(ParagraphAlign paragraph_align) { _paragraph_align = paragraph_align; return *this; }
		constexpr ParagraphFormat& flow_direction(FlowDirection text_direction) { _flow_direction = text_direction; return *this; }
		constexpr ParagraphFormat& read_direction(ReadDirection read_direction) { _read_direction = read_direction; return *this; }
		constexpr ParagraphFormat& word_wrap(WordWrap word_wrap) { _word_wrap = word_wrap; return *this; }
		constexpr ParagraphFormat& line_height(ValueTag line_height) { _line_height = line_height; return *this; }
		constexpr ParagraphFormat& tab_size(ValueTag tab_size) { _tab_size = tab_size; return *this; }
	}paragraph;

	struct FontFormat {
	public:
		std::wstring _family = L"Segoe UI";
		std::wstring _locale = L"";
		FontWeight _weight = FontWeight::Normal;
		FontStyle _style = FontStyle::Normal;
		FontStretch _stretch = FontStretch::Normal;
		bool _underline = false;
		bool _strikeline = false;
		float _size = 16.0f;
		Color _color = Color::Black;
	public:
		FontFormat& family(std::wstring family) { _family = family; return *this; }
		FontFormat& locale(std::wstring locale) { _locale = locale; return *this; }
		constexpr FontFormat& weight(FontWeight weight) { _weight = weight; return *this; }
		constexpr FontFormat& style(FontStyle style) { _style = style; return *this; }
		constexpr FontFormat& stretch(FontStretch stretch) { _stretch = stretch; return *this; }
		constexpr FontFormat& size(float size) { _size = size; return *this; }
		constexpr FontFormat& color(Color color) { _color = color; return *this; }
		constexpr FontFormat& underline(bool underline) { _underline = underline; return *this; }
		constexpr FontFormat& strikeline(bool strikeline) { _strikeline = strikeline; return *this; }
	}font;
};


struct TextBlockHitTestInfo {
	uint text_position = 0;
	uint text_length = 0;
	bool is_inside = false;
	bool is_trailing_hit = false;
	Rect geometry_region = region_empty;
};


END_NAMESPACE(WndDesign)