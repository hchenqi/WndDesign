#pragma once

#include "d2d_target.h"


struct IDXGISwapChain1;

struct IDCompositionTarget;
struct IDCompositionVisual;


BEGIN_NAMESPACE(WndDesign)

using HANDLE = void*;


class HWNDTarget : public Target {
private:
	alloc_ptr<IDXGISwapChain1> swap_chain;
	alloc_ptr<IDCompositionTarget> comp_target;
	alloc_ptr<IDCompositionVisual> comp_visual;
	void CreateTarget();
	void DestroyTarget();
public:
	HWNDTarget(HANDLE hwnd, Size size);
	~HWNDTarget();
	void OnResize(Size size);
	void Present();
};


END_NAMESPACE(WndDesign)