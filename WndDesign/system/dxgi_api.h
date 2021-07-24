#pragma once

#include "../common/core.h"


struct IDXGIDevice1;
struct IDXGIAdapter;
struct IDXGIFactory2;


BEGIN_NAMESPACE(WndDesign)


IDXGIDevice1& GetDXGIDevice();
IDXGIAdapter& GetDXGIAdapter();
IDXGIFactory2& GetDXGIFactory();


END_NAMESPACE(WndDesign)