#include "region.h"

#include <Windows.h>
#include <windowsx.h>


BEGIN_NAMESPACE(WndDesign)


inline Rect AsRect(RECT rect) {
	return Rect((float)rect.left, (float)rect.top, (float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
}


static Region& TempRegion(Rect rect) {
	static Region region; region.Set(rect); return region;
}


Region::Region(Rect region) :
	rgn(CreateRectRgn((int)floorf(region.left()), (int)floorf(region.top()), (int)ceilf(region.right()), (int)ceilf(region.bottom()))) {
}

Region::~Region() {
	DeleteObject(rgn);
}

bool Region::IsEmpty() const {
	return CombineRgn((HRGN)rgn, (HRGN)rgn, NULL, RGN_COPY) == NULLREGION;
}

void Region::Set(Rect region) {
	SetRectRgn((HRGN)rgn, (int)floorf(region.left()), (int)floorf(region.top()), (int)ceilf(region.right()), (int)ceilf(region.bottom()));
}

void Region::Translate(Vector vector) {
	OffsetRgn((HRGN)rgn, (int)roundf(vector.x), (int)roundf(vector.y));
}

void Region::Union(const Region& region) {
	CombineRgn((HRGN)rgn, (HRGN)rgn, (HRGN)region.rgn, RGN_OR);
}

void Region::Intersect(const Region& region) {
	CombineRgn((HRGN)rgn, (HRGN)rgn, (HRGN)region.rgn, RGN_AND);
}

void Region::Sub(const Region& region) {
	CombineRgn((HRGN)rgn, (HRGN)rgn, (HRGN)region.rgn, RGN_DIFF);
}

void Region::Xor(const Region& region) {
	CombineRgn((HRGN)rgn, (HRGN)rgn, (HRGN)region.rgn, RGN_XOR);
}

void Region::Union(const Rect& region) { Union(TempRegion(region)); }
void Region::Intersect(const Rect& region) { Intersect(TempRegion(region)); }
void Region::Sub(const Rect& region) { Sub(TempRegion(region)); }
void Region::Xor(const Rect& region) { Xor(TempRegion(region)); }

Rect Region::GetBoundingRect() const {
	RECT rect;
	GetRgnBox((HRGN)rgn, &rect);
	return AsRect(rect);
}

std::pair<Rect, std::vector<Rect>> Region::GetRects() const {
	static std::vector<char> buffer;
	int size = GetRegionData((HRGN)rgn, 0, NULL); buffer.resize(size);
	GetRegionData((HRGN)rgn, size, (LPRGNDATA)buffer.data());
	RGNDATA& data = *(LPRGNDATA)buffer.data();
	static_assert(sizeof(RECT) == sizeof(Rect));
	Rect bound = AsRect(data.rdh.rcBound);
	std::vector<Rect> regions((Rect*)(buffer.data() + data.rdh.dwSize), (Rect*)(buffer.data() + data.rdh.dwSize) + data.rdh.nCount);
	for (auto& region : regions) { region = AsRect(*reinterpret_cast<RECT*>(&region)); }
	return { bound , regions };
}


END_NAMESPACE(WndDesign)