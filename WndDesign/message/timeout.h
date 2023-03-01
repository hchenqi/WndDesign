#pragma once

#include "timer.h"
#include "../common/coroutine.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


class Timeout : private Timer {
public:
	Timeout() : Timer([&]() { function ? Reset()() : Stop(); }) {}
	~Timeout() {}

private:
	std::function<void(void)> function;
private:
	std::function<void(void)> Reset() { return std::move(function); }

public:
	void Set(std::function<void(void)> function, uint delay) { this->function = function; Timer::Set(delay); }  // in milliseconds
	void Cancel() { Stop(); Reset(); }
};


inline std::shared_ptr<Timeout> SetTimeout(std::function<void(void)> function, uint delay) {
	std::shared_ptr<Timeout> timeout(new Timeout());
	timeout->Set([timeout, function]() { function(); }, delay);
	return timeout;
}


inline Task<> StartTimeout(uint delay) {
	return StartTask([=](resolver<> resolve) { SetTimeout([=]() { resolve(); }, delay); });
}


END_NAMESPACE(WndDesign)