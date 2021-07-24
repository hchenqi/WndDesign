#pragma once

#include "core.h"


BEGIN_NAMESPACE(WndDesign)


class Uncopyable {
protected:
	Uncopyable() = default;
	~Uncopyable() = default;
private:
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator=(const Uncopyable&) = delete;
};


END_NAMESPACE(WndDesign)