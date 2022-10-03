#pragma once

#include "../common/uncopyable.h"

#include <functional>


BEGIN_NAMESPACE(WndDesign)


class Timer : public Uncopyable {
private:
	using HANDLE = void*;
	HANDLE timer;
public:
	std::function<void(void)> callback;
public:
	Timer(std::function<void(void)> callback) : timer(nullptr), callback(callback) {}
	~Timer() { Stop(); }
public:
	bool IsSet() const { return timer != nullptr; }
	void Set(uint period);  // in milliseconds
	void Stop();
};


END_NAMESPACE(WndDesign)