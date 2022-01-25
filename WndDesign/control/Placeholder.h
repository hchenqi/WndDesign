#pragma once

#include "../window/wnd_traits.h"


BEGIN_NAMESPACE(WndDesign)


template<class WidthType, class HeightType>
class Placeholder;


template<>
class Placeholder<Auto, Auto> : public WndType<Auto, Auto> {
public:
	Placeholder(Size size) : size(size) {}
private:
	Size size;
public:
	void SetSize(Size size) { if (this->size != size) { this->size = size; SizeUpdated(); } }
protected:
	virtual Size GetSize() override { return size; }
};


template<>
class Placeholder<Auto, Assigned> : public WndType<Auto, Assigned> {
public:
	Placeholder(uint width) : size(width, 0) {}
private:
	Size size;
public:
	void SetWidth(uint width) { if (size.width != width) { size.width = width; SizeUpdated(); } }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size.height = size_ref.height; }
	virtual Size GetSize() override { return size; }
};


template<>
class Placeholder<Assigned, Auto> : public WndType<Assigned, Auto> {
public:
	Placeholder(uint height) : size(0, height) {}
private:
	Size size;
public:
	void SetHeight(uint height) { if (size.height != height) { size.height = height; SizeUpdated(); } }
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size.width = size_ref.width; }
	virtual Size GetSize() override { return size; }
};


template<>
class Placeholder<Assigned, Assigned> : public WndType<Assigned, Assigned> {
private:
	Size size;
protected:
	virtual void OnSizeRefUpdate(Size size_ref) override { size = size_ref; }
	virtual Size GetSize() override { return size; }
};


END_NAMESPACE(WndDesign)