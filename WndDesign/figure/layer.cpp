#include "layer.h"
#include "figure_queue.h"

#include "../system/d2d_api.h"
#include "../system/directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


inline alloc_ptr<ID2D1Bitmap1> D2DCreateBitmap(Size size) {
    D2D1_BITMAP_PROPERTIES1 bitmap_properties = D2D1::BitmapProperties1(
        D2D1_BITMAP_OPTIONS_TARGET,
        D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
    );
    ComPtr<ID2D1Bitmap1> bitmap;
    hr << GetD2DDeviceContext().CreateBitmap(D2D1::SizeU(size.width, size.height), nullptr, 0, &bitmap_properties, &bitmap);
    return bitmap.Detach();
}


Layer::Layer(Size size) : bitmap(D2DCreateBitmap(size)) {}

Layer::~Layer() { SafeRelease(&bitmap); }

void Layer::DrawFigureQueue(const FigureQueue& figure_queue, Vector offset, Rect clip_region) {
    figure_queue.CheckFigureGroup(); if (bitmap == nullptr) { return; }
    ID2D1DeviceContext& device_context = GetD2DDeviceContext(); device_context.SetTarget(static_cast<ID2D1Bitmap1*>(bitmap));
    device_context.PushAxisAlignedClip(Rect2RECT(clip_region), D2D1_ANTIALIAS_MODE_ALIASED);
    device_context.Clear(Color2COLOR(color_transparent));
    auto& groups = figure_queue.GetFigureGroups();
    auto& figures = figure_queue.GetFigures(); uint figure_index = 0;
    for (uint group_index = 1; group_index < groups.size(); ++group_index) {
        auto& group = groups[group_index];
        for (; figure_index < group.figure_index; ++figure_index) {
            figures[figure_index].figure->DrawOn(static_cast<RenderTarget&>(device_context), figures[figure_index].offset + offset);
        }
        if (group.IsBegin()) {
            // For group begin:
            // Calculate new coordinate offset and clip region.
            Vector new_offset = offset + group.offset;
            Rect new_clip_region = clip_region.Intersect(group.clip_region + new_offset);
            // Jump to group end if new clip region is empty.
            auto& group_end = groups[group.group_end_index];
            if (new_clip_region.IsEmpty()) {
                group_index = group.group_end_index;
                figure_index = group_end.figure_index;
                continue;
            }
            // Store old offset and clip region to group end.
            group_end.prev_offset = offset;
            group_end.prev_clip_region = clip_region;
            // Set new offset and clip region.
            offset = new_offset;
            clip_region = new_clip_region;
            device_context.PushAxisAlignedClip(Rect2RECT(clip_region), D2D1_ANTIALIAS_MODE_ALIASED);
        } else {
            // For group end:
            // Restore previous offset and clip region.
            offset = group.prev_offset;
            clip_region = group.prev_clip_region;
            device_context.PopAxisAlignedClip();
        }
    }
}


void LayerFigure::DrawOn(RenderTarget& target, Point point) const {
	if (layer.bitmap == nullptr) { return; }
	target.DrawBitmap(
		static_cast<ID2D1Bitmap1*>(layer.bitmap),
		Rect2RECT(Rect(point, region.size)),
		1.0,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		Rect2RECT(region)
	);
}


END_NAMESPACE(WndDesign)