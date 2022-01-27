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


void Layer::Create(Size size) {
	bitmap.Set(static_cast<BitmapResource*>(D2DCreateBitmap(size)));
}

void Layer::DrawFigureQueue(const FigureQueue& figure_queue, Vector offset, Rect clip_region) {
	ID2D1DeviceContext& device_context = GetD2DDeviceContext(); device_context.SetTarget(bitmap.Get());
	Transform transform = Transform::Translation((float)offset.x, (float)offset.y);
	device_context.SetTransform(AsD2DTransform(transform));
	device_context.PushAxisAlignedClip(Rect2RECT(clip_region), D2D1_ANTIALIAS_MODE_ALIASED);
	device_context.Clear(Color2COLOR(color_transparent));
	auto& groups = figure_queue.GetFigureGroups();
	auto& figures = figure_queue.GetFigures(); uint figure_index = 0;
	for (uint group_index = 1; group_index < groups.size(); ++group_index) {
		auto& group = groups[group_index];
		for (; figure_index < group.figure_index; ++figure_index) {
			figures[figure_index].figure->DrawOn(static_cast<RenderTarget&>(device_context), figures[figure_index].offset);
		}
		if (group.IsBegin()) {
			groups[group.group_end_index].prev_transform = transform;
			device_context.SetTransform(AsD2DTransform(transform = group.transform * transform));
			device_context.PushAxisAlignedClip(Rect2RECT(group.clip_region), D2D1_ANTIALIAS_MODE_ALIASED);
		} else {
			transform = group.prev_transform;
			device_context.PopAxisAlignedClip();
			device_context.SetTransform(AsD2DTransform(transform));
		}
	}
}


void LayerFigure::DrawOn(RenderTarget& target, Point point) const {
	target.DrawBitmap(
		layer.bitmap.Get(),
		Rect2RECT(Rect(point, region.size)),
		1.0,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		Rect2RECT(region)
	);
}


END_NAMESPACE(WndDesign)