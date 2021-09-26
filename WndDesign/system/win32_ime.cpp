#include "win32_ime.h"

#include <Windows.h>


#pragma comment(lib, "imm32.lib")


BEGIN_NAMESPACE(WndDesign)


#pragma warning (push)
#pragma warning (disable : 6387)   // '_Param_(2)' could be '0': this does not adhere to the specification for the function 'ImmAssociateContextEx'
void ImeEnable(HANDLE hwnd) {
	ImmAssociateContextEx((HWND)hwnd, nullptr, IACE_DEFAULT);
}

void ImeDisable(HANDLE hwnd) {
	ImmAssociateContextEx((HWND)hwnd, nullptr, 0);
}
#pragma warning (pop)

void ImeSetPosition(HANDLE hwnd, Point point) {
	HIMC imc = ImmGetContext((HWND)hwnd);
	COMPOSITIONFORM cf = { CFS_POINT, { point.x, point.y } };
	ImmSetCompositionWindow(imc, &cf);
	ImmReleaseContext((HWND)hwnd, imc);
}

wstring ImeGetString(HANDLE hwnd, uint type) {
	if (type & GCS_RESULTSTR) { type = GCS_RESULTSTR; } //
	else if (type & GCS_COMPSTR) { type = GCS_COMPSTR; } //
	else { return wstring(); }
	HIMC imc = ImmGetContext((HWND)hwnd);
	uint size = ImmGetCompositionStringW(imc, type, nullptr, 0);
	wstring result; result.resize(size / sizeof(wchar));
	ImmGetCompositionStringW(imc, type, result.data(), size);
	ImmReleaseContext((HWND)hwnd, imc);
	return result;
}


END_NAMESPACE(WndDesign)