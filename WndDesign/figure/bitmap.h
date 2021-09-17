#pragma once

#include "../common/uncopyable.h"


BEGIN_NAMESPACE(WndDesign)

struct BitmapResource;


class Bitmap : Uncopyable {
private:
	mutable alloc_ptr<BitmapResource> bitmap;
public:
	Bitmap();
	~Bitmap();
	bool IsEmpty() const { return bitmap == nullptr; }
	void Set(alloc_ptr<BitmapResource> bitmap) const { Destroy(); this->bitmap = bitmap; }
	void Destroy() const;
	ref_ptr<BitmapResource> Get() const { if (IsEmpty()) { throw std::invalid_argument("invalid bitmap resource"); } return bitmap; }
};


END_NAMESPACE(WndDesign)