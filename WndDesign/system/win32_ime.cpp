#include "win32_ime.h"

#include <Windows.h>


#pragma comment(lib, "imm32.lib")


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

std::wstring ime_string;
size_t cursor_position;

END_NAMESPACE(Anonymous)


#pragma warning (push)
#pragma warning (disable : 6387)  // '_Param_(2)' could be '0': this does not adhere to the specification for the function 'ImmAssociateContextEx'
void ImeEnable(HANDLE hwnd) {
	ImmAssociateContextEx((HWND)hwnd, nullptr, IACE_DEFAULT);
}

void ImeDisable(HANDLE hwnd) {
	ImmAssociateContextEx((HWND)hwnd, nullptr, 0);
}
#pragma warning (pop)

void ImeSetPosition(HANDLE hwnd, Point point) {
	HIMC imc = ImmGetContext((HWND)hwnd);
	CANDIDATEFORM cf = { 0, CFS_CANDIDATEPOS, { (int)point.x, (int)point.y }, {} };
	ImmSetCandidateWindow(imc, &cf);
	ImmReleaseContext((HWND)hwnd, imc);
}

void ImeUpdateString(HANDLE hwnd, uint type) {
	HIMC imc = ImmGetContext((HWND)hwnd);
	if (type & GCS_COMPSTR) {
		uint size = ImmGetCompositionStringW(imc, GCS_COMPSTR, nullptr, 0);
		ime_string.resize(size / sizeof(wchar));
		ImmGetCompositionStringW(imc, GCS_COMPSTR, ime_string.data(), size);
	}
	if (type & GCS_RESULTSTR) {
		uint size = ImmGetCompositionStringW(imc, GCS_RESULTSTR, nullptr, 0);
		ime_string.resize(size / sizeof(wchar));
		ImmGetCompositionStringW(imc, GCS_RESULTSTR, ime_string.data(), size);
	}
	if (type & GCS_CURSORPOS) {
		cursor_position = ImmGetCompositionString(imc, GCS_CURSORPOS, NULL, 0);
	}
	ImmReleaseContext((HWND)hwnd, imc);
}


std::wstring ImeGetString() { return std::move(ime_string); }

size_t ImeGetCursorPosition() { return cursor_position; }


END_NAMESPACE(WndDesign)