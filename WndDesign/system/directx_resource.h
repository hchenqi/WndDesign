#pragma once

#include "../common/core.h"


BEGIN_NAMESPACE(WndDesign)


class Bitmap;

void RegisterBitmap(Bitmap& bitmap);
void UnregisterBitmap(Bitmap& bitmap);

void DirectXRecreateResource();


END_NAMESPACE(WndDesign)