#pragma once

#include "timer.h"

#include <memory>


BEGIN_NAMESPACE(WndDesign)


class Timeout : private Timer {
public:
	Timeout() : Timer([&]() { Stop(); this->function(); Reset(); }) {}
	~Timeout() {}

private:
	std::function<void(void)> function;
private:
	void Reset() { std::function<void(void)> temp(std::move(function)); }

public:
	void Set(std::function<void(void)> function, uint delay) { this->function = function; Timer::Set(delay); }  // in milliseconds
	void Cancel() { Stop(); Reset(); }
};


inline std::shared_ptr<Timeout> SetTimeout(std::function<void(void)> function, uint delay) {
	std::shared_ptr<Timeout> timeout(new Timeout());
	timeout->Set([timeout, function]() { function(); }, delay);
	return timeout;
}


END_NAMESPACE(WndDesign)