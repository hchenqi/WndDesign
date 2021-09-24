#pragma once

#include "uncopyable.h"

#include <string>


BEGIN_NAMESPACE(WndDesign)


enum class UTF16CharType : uchar {
	Single,
	SurrogateLow,
	SurrogateHigh,
};

inline UTF16CharType GetUTF16CharType(wchar ch) {
	if (ch >= L'\xD800' && ch <= L'\xDBFF') { return UTF16CharType::SurrogateLow; }
	if (ch >= L'\xDC00' && ch <= L'\xDFFF') { return UTF16CharType::SurrogateHigh; }
	return UTF16CharType::Single;
}

inline uint GetUTF16CharLength(wchar ch) {
	switch (GetUTF16CharType(ch)) {
	case UTF16CharType::Single: return 1;
	case UTF16CharType::SurrogateLow:
	case UTF16CharType::SurrogateHigh: return 2;
	default: assert(false); return 0;
	}
}


struct TextRange {
public:
	uint begin; uint length;
public:
	uint left() const { return begin; }
	uint right() const { return begin + length; }
	bool IsEmpty() const { return length == 0; }
	bool Contains(uint pos) const { return pos >= left() && pos < right(); }
};


class WordBreakIterator : public Uncopyable {
private:
	alloc_ptr<void> iter;
	ref_ptr<const wchar> str; uint length;
	uint begin, end;
public:
	WordBreakIterator();
	~WordBreakIterator();
public:
	void SetText(const std::wstring& str);
	TextRange Seek(uint pos);
	TextRange Next();
	TextRange Prev();
};


END_NAMESPACE(WndDesign)