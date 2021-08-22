#pragma once

#include "wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class Placeholder;


template<>
class Placeholder<Auto, Auto> : public WndType<int, Auto> {
public:
	Placeholder(Size size) : size(size) {}
private:
	Size size;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { return size; }
public:
	void SetSize(Size size) { if (this->size != size) { this->size = size; SizeUpdated(size); } }
};


template<>
class Placeholder<Relative, Auto> : public WndType<Relative, Auto> {
public:
	Placeholder(uint height) : size(length_min, height) {}
private:
	Size size;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { size.width = size_ref.width; return size; }
public:
	void SetHeight(uint height) { if (size.height != height) { size.height = height; SizeUpdated(size); } }
};


template<>
class Placeholder<Auto, Relative> : public WndType<Auto, Relative> {
public:
	Placeholder(uint width) : size(width, length_min) {}
private:
	Size size;
private:
	virtual const Size OnSizeRefUpdate(Size size_ref) override { size.height = size_ref.height; return size; }
public:
	void SetWidth(uint width) { if (size.width != width) { size.width = width; SizeUpdated(size); } }
};


template<>
class Placeholder<Relative, Relative> : public WndType<Relative, Relative> {};


END_NAMESPACE(WndDesign)