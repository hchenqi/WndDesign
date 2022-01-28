#include "figure.h"
#include "../system/directx_helper.h"


BEGIN_NAMESPACE(WndDesign)


const Size GetTargetSize(const RenderTarget& target) {
	return AsSize(target.GetSize());
}


END_NAMESPACE(WndDesign)