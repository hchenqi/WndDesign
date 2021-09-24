#include "unicode_helper.h"

#include <icu.h>

#pragma comment(lib, "icu.lib")


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

UErrorCode status = U_ZERO_ERROR;

inline UBreakIterator* AsUBreakIterator(void* iter) { return static_cast<UBreakIterator*>(iter); }

END_NAMESPACE(Anonymous)

static_assert(sizeof(wchar_t) == sizeof(UChar));


WordBreakIterator::WordBreakIterator() {
	iter = ubrk_open(UBRK_WORD, nullptr, nullptr, 0, &status); assert(U_SUCCESS(status));
	str = nullptr; length = 0;
	begin = end = UBRK_DONE;
}

WordBreakIterator::~WordBreakIterator() {
	ubrk_close(AsUBreakIterator(iter));
}

void WordBreakIterator::SetText(const std::wstring& str) {
	ubrk_setText(AsUBreakIterator(iter), (const UChar*)str.c_str(), (uint)str.length(), &status); assert(U_SUCCESS(status));
	this->str = str.c_str(); this->length = (uint)str.length();
	begin = end = UBRK_DONE;
}

TextRange WordBreakIterator::Seek(uint pos) {
	if (str == nullptr || pos >= length) { throw std::invalid_argument("invalid text position"); }
	begin = ubrk_preceding(AsUBreakIterator(iter), pos + 1); assert(begin != UBRK_DONE);
	end = ubrk_next(AsUBreakIterator(iter)); assert(end != UBRK_DONE);
	assert(end > begin);
	return { begin, end - begin };
}

TextRange WordBreakIterator::Next() {
	if (str == nullptr || begin == -1) { throw std::invalid_argument("invalid text position"); }
	if (end >= length) { throw std::invalid_argument("cannot increment iterator past end"); }
	begin = end;
	end = ubrk_next(AsUBreakIterator(iter)); assert(end != UBRK_DONE);
	assert(end > begin);
	return { begin, end - begin };
}

TextRange WordBreakIterator::Prev() {
	if (str == nullptr || begin == -1) { throw std::invalid_argument("invalid text position"); }
	if (begin == 0) { throw std::invalid_argument("cannot decrement iterator before begin"); }
	end = begin;
	begin = ubrk_preceding(AsUBreakIterator(iter), begin); assert(begin != UBRK_DONE);
	assert(end > begin);
	return { begin, end - begin };
}


END_NAMESPACE(WndDesign)