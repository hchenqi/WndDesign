#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class Placeholder;


template<>
class Placeholder<Auto, Auto> : public WndType<Auto, Auto> {
public:
	Placeholder(Size size) : size(size) {}
protected:
	Size size;
public:
	void SetSize(Size size) { if (this->size != size) { this->size = size; SizeUpdated(); } }
protected:
	virtual Size GetSize() override { return size; }
};


template<>
class Placeholder<Auto, Assigned> : public WndType<Auto, Assigned> {
public:
	Placeholder(float width) : size(width, 0.0f) {}
protected:
	Size size;
public:
	void SetWidth(float width) { if (size.width != width) { size.width = width; SizeUpdated(); } }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size.height = size_ref.height; }
	virtual Size GetSize() override { return size; }
};


template<>
class Placeholder<Assigned, Auto> : public WndType<Assigned, Auto> {
public:
	Placeholder(float height) : size(0.0f, height) {}
protected:
	Size size;
public:
	void SetHeight(float height) { if (size.height != height) { size.height = height; SizeUpdated(); } }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size.width = size_ref.width; }
	virtual Size GetSize() override { return size; }
};


template<>
class Placeholder<Assigned, Assigned> : public WndType<Assigned, Assigned> {
protected:
	Size size;
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size = size_ref; }
	virtual Size GetSize() override { return size; }
};


END_NAMESPACE(WndDesign)