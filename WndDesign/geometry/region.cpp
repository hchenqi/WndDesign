#include "region.h"

#include <Windows.h>
#include <windowsx.h>


BEGIN_NAMESPACE(WndDesign)


inline Rect RECT2Rect(RECT rect) {
    return Rect(rect.left, rect.top, (uint)(rect.right - rect.left), (uint)(rect.bottom - rect.top));
}


static Region& TempRegion(Rect rect) {
    static Region region; region.Set(rect); return region;
}


Region::Region(Rect region) : 
    rgn(CreateRectRgn(region.left(), region.top(), region.right(), region.bottom())) {
}

Region::~Region() {
    DeleteObject(rgn);
}

bool Region::IsEmpty() const {
    return CombineRgn((HRGN)rgn, (HRGN)rgn, NULL, RGN_COPY) == NULLREGION;
}

void Region::Set(Rect region) {
    SetRectRgn((HRGN)rgn, region.left(), region.top(), region.right(), region.bottom());
}

void Region::Translate(Vector vector) {
    OffsetRgn((HRGN)rgn, vector.x, vector.y);
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
    return RECT2Rect(rect);
}

std::pair<Rect, std::vector<Rect>> Region::GetRects() const {
    static std::vector<char> buffer;
    int size = GetRegionData((HRGN)rgn, 0, NULL); buffer.resize(size);
    GetRegionData((HRGN)rgn, size, (LPRGNDATA)buffer.data());
    RGNDATA& data = *(LPRGNDATA)buffer.data();
    static_assert(sizeof(RECT) == sizeof(Rect));
    Rect bound = RECT2Rect(data.rdh.rcBound);
    std::vector<Rect> regions((Rect*)(buffer.data() + data.rdh.dwSize), (Rect*)(buffer.data() + data.rdh.dwSize) + data.rdh.nCount);
    for (auto& region : regions) { region = RECT2Rect(*reinterpret_cast<RECT*>(&region)); }
    return { bound , regions };
}


END_NAMESPACE(WndDesign)