#include "win32_api.h"
#include "win32_ime.h"
#include "../window/desktop.h"
#include "../frame/DesktopFrame.h"

#include <Windows.h>
#include <windowsx.h>


BEGIN_NAMESPACE(WndDesign)

struct DesktopFrameApi : DesktopFrame {
	DesktopFrame::GetMinMaxRegion;
	DesktopFrame::GetRegion;
	DesktopFrame::SetScale;
	DesktopFrame::SetSize;
	DesktopFrame::SetPoint;
	DesktopFrame::Status;
	DesktopFrame::SetStatus;
	DesktopFrame::Destroy;
	DesktopFrame::Draw;
	DesktopFrame::LoseTrack;
	DesktopFrame::LoseCapture;
	DesktopFrame::DispatchMouseMsg;
};

BEGIN_NAMESPACE(Anonymous)

inline RECT AsWin32Rect(Rect rect) { return { (int)floorf(rect.left()), (int)floorf(rect.top()), (int)ceilf(rect.right()), (int)ceilf(rect.bottom()) }; }

inline Rect AsRect(RECT rect) { return Rect((float)rect.left, (float)rect.top, (float)(rect.right - rect.left), (float)(rect.bottom - rect.top)); }

inline Size GetWorkAreaSize() { RECT rect; SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0); return AsRect(rect).size; }

Size desktop_size = GetWorkAreaSize();

constexpr float dpi_default = 96.0f;

inline bool IsMouseMsg(UINT msg) { return WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST; }
inline bool IsKeyboardMsg(UINT msg) { return WM_KEYFIRST <= msg && msg <= WM_KEYLAST; }

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	static bool is_mouse_tracked = false;

	ref_ptr<DesktopFrameApi> frame = reinterpret_cast<ref_ptr<DesktopFrameApi>>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (frame == nullptr) { goto FrameIrrelevantMessages; }

	// mouse message
	if (IsMouseMsg(msg)) {
		MouseMsg mouse_msg;
		mouse_msg.point = Point((float)GET_X_LPARAM(lparam), (float)GET_Y_LPARAM(lparam));
		mouse_msg._key_state = (uchar)GET_KEYSTATE_WPARAM(wparam);;
		mouse_msg.wheel_delta = GET_WHEEL_DELTA_WPARAM(wparam);
		switch (msg) {
		case WM_MOUSEMOVE: mouse_msg.type = MouseMsg::Move;
			if (!is_mouse_tracked) {
				TRACKMOUSEEVENT track_mouse_event;
				track_mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
				track_mouse_event.dwFlags = TME_LEAVE;
				track_mouse_event.hwndTrack = hwnd;
				track_mouse_event.dwHoverTime = HOVER_DEFAULT;
				TrackMouseEvent(&track_mouse_event);
				is_mouse_tracked = true;
			}
			break;
		case WM_LBUTTONDOWN: mouse_msg.type = MouseMsg::LeftDown; break;
		case WM_LBUTTONUP: mouse_msg.type = MouseMsg::LeftUp; break;
		case WM_RBUTTONDOWN: mouse_msg.type = MouseMsg::RightDown; break;
		case WM_RBUTTONUP: mouse_msg.type = MouseMsg::RightUp; break;
		case WM_MBUTTONDOWN: mouse_msg.type = MouseMsg::MiddleDown; break;
		case WM_MBUTTONUP: mouse_msg.type = MouseMsg::MiddleUp; break;
		case WM_MOUSEWHEEL: mouse_msg.type = MouseMsg::WheelVertical; mouse_msg.point -= frame->GetRegion().point - point_zero; break;
		case WM_MOUSEHWHEEL: mouse_msg.type = MouseMsg::WheelHorizontal; mouse_msg.point -= frame->GetRegion().point - point_zero; break;
		default: return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		frame->DispatchMouseMsg(mouse_msg);
		return 0;
	}

	// keyboard message
	if (IsKeyboardMsg(msg)) {
		KeyMsg key_msg;
		key_msg.key = static_cast<Key>(wparam);
		key_msg.ch = static_cast<wchar>(wparam);
		switch (msg) {
		case WM_KEYDOWN: key_msg.type = KeyMsg::KeyDown; break;
		case WM_KEYUP: key_msg.type = KeyMsg::KeyUp; break;
		case WM_CHAR: key_msg.type = KeyMsg::Char; break;
		default: return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		desktop.DispatchKeyMsg(*frame, key_msg);
		return 0;
	}

	{
		switch (msg) {
			// ime message
		case WM_IME_SETCONTEXT: break;
		case WM_IME_STARTCOMPOSITION: desktop.OnImeCompositionBegin(); break;
		case WM_IME_COMPOSITION: desktop.OnImeComposition(ImeGetString(hwnd, (uint)lparam)); break;
		case WM_IME_ENDCOMPOSITION: desktop.OnImeCompositionEnd(); break;

			// region message
		case WM_GETMINMAXINFO: {
			MINMAXINFO* min_max_info = reinterpret_cast<MINMAXINFO*>(lparam);
			auto [size_min, region_max] = frame->GetMinMaxRegion();
			min_max_info->ptMaxPosition = { (int)floorf(region_max.point.x), (int)floorf(region_max.point.y) };
			min_max_info->ptMaxSize = { (int)ceilf(region_max.size.width), (int)ceilf(region_max.size.height) };
			min_max_info->ptMinTrackSize = { (int)floorf(size_min.width), (int)floorf(size_min.height) };
			min_max_info->ptMaxTrackSize = min_max_info->ptMaxSize;
		}break;
		case WM_WINDOWPOSCHANGING: break;
		case WM_WINDOWPOSCHANGED: return DefWindowProc(hwnd, msg, wparam, lparam);
		case WM_MOVE: frame->SetPoint(Point((short)LOWORD(lparam), (short)HIWORD(lparam))); break;
		case WM_SIZE: frame->SetSize(Size(LOWORD(lparam), HIWORD(lparam)));
			frame->SetStatus(static_cast<DesktopFrameApi::Status>(wparam <= 2 ? wparam : 2)); break;

			// notifications
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			frame->Draw();
			EndPaint(hwnd, &ps);
		}break;
		case WM_ERASEBKGND: return true;
		case WM_MOUSELEAVE: is_mouse_tracked = false; frame->LoseTrack(); break;
		case WM_CAPTURECHANGED: frame->LoseCapture(); break;
		case WM_KILLFOCUS: desktop.LoseFocus(); break;

		case WM_DPICHANGED: frame->SetScale(LOWORD(wparam) / dpi_default); break;

			// convert scroll message to mouse wheel message
		case WM_HSCROLL:
		case WM_VSCROLL:
		{
			POINT cursor_position;
			GetCursorPos(&cursor_position);
			short key_state = 0;
			if (GetAsyncKeyState(VK_SHIFT)) { key_state |= MK_SHIFT; }
			if (GetAsyncKeyState(VK_CONTROL)) { key_state |= MK_CONTROL; }
			short wheel_delta = 0;
			switch (LOWORD(wparam)) {
			case SB_LINEUP: case SB_PAGEUP: wheel_delta = WHEEL_DELTA; break;
			case SB_LINEDOWN: case SB_PAGEDOWN: wheel_delta = -WHEEL_DELTA; break;
			default: return 0;
			}
			return WndProc(
				hwnd, msg == WM_HSCROLL ? WM_MOUSEHWHEEL : WM_MOUSEWHEEL,
				(wheel_delta << 16) | key_state, ((short)cursor_position.y << 16) | (short)cursor_position.x
			);
		}
		default: goto FrameIrrelevantMessages;
		}
		return 0;
	}

FrameIrrelevantMessages:
	switch (msg) {
	case WM_CREATE: break;
	case WM_DESTROY: if (frame != nullptr) { frame->Destroy(); }  break;

	case WM_NCCALCSIZE: break;
	case WM_NCHITTEST: return HTCLIENT;
	case WM_NCPAINT: break;
	case WM_NCACTIVATE: return true;

	case WM_SETTINGCHANGE:
		if (wparam == SPI_SETWORKAREA) { desktop_size = GetWorkAreaSize(); break; }
		[[fallthrough]];

	default: return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}

static const wchar_t wnd_class_name[] = L"WndDesignFrame";
HINSTANCE hInstance = NULL;

inline void RegisterWndClass() {
	static bool has_wnd_class_registered = false;
	if (!has_wnd_class_registered) {
		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.lpfnWndProc = WndProc;
		wcex.hInstance = (hInstance = GetModuleHandle(NULL));
		wcex.lpszClassName = wnd_class_name;
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		ATOM res = RegisterClassExW(&wcex);
		if (res == 0) { throw std::runtime_error("register class error"); }
		has_wnd_class_registered = true;
	}
}

END_NAMESPACE(Anonymous)

BEGIN_NAMESPACE(Win32)


Size GetDesktopSize() { return desktop_size; }

HANDLE CreateWnd(Rect region, std::wstring title) {
	RegisterWndClass();
	HWND hwnd = CreateWindowExW(WS_EX_NOREDIRECTIONBITMAP, wnd_class_name, title.c_str(),
								WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_HSCROLL | WS_VSCROLL,
								(int)floorf(region.point.x), (int)floorf(region.point.y), (int)ceilf(region.size.width), (int)ceilf(region.size.height),
								NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) { throw std::runtime_error("create window error"); }
	return hwnd;
}

void DestroyWnd(HANDLE hwnd) { DestroyWindow((HWND)hwnd); }

void SetWndUserData(HANDLE hwnd, void* data) {
	SetWindowLongPtrW((HWND)hwnd, GWLP_USERDATA, (LONG_PTR)data);
}

float GetWndDpiScale(HANDLE hwnd) {
	return GetDpiForWindow((HWND)hwnd) / dpi_default;
}

void SetWndTitle(HANDLE hwnd, std::wstring title) {
	SetWindowTextW((HWND)hwnd, title.c_str());
}

void SetWndRegion(HANDLE hwnd, Rect region) {
	MoveWindow((HWND)hwnd, (int)floorf(region.point.x), (int)floorf(region.point.y), (int)ceilf(region.size.width), (int)ceilf(region.size.height), false);
}

void ShowWnd(HANDLE hwnd) { ShowWindow((HWND)hwnd, SW_SHOWDEFAULT); }
void MinimizeWnd(HANDLE hwnd) { ShowWindow((HWND)hwnd, SW_MINIMIZE); }
void MaximizeWnd(HANDLE hwnd) { ShowWindow((HWND)hwnd, SW_MAXIMIZE); }
void RestoreWnd(HANDLE hwnd) { ShowWindow((HWND)hwnd, SW_RESTORE); }

void InvalidateWndRegion(HANDLE hwnd, Rect region) {
	RECT rect = AsWin32Rect(region);
	InvalidateRect((HWND)hwnd, &rect, false);
}

void SetCapture(HANDLE hwnd) { ::SetCapture((HWND)hwnd); }
void ReleaseCapture() { ::ReleaseCapture(); }
void SetFocus(HANDLE hwnd) { ::SetFocus((HWND)hwnd); }

int MessageLoop() {
	MSG msg;
	while (GetMessageW(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return (int)msg.wParam;
}

void Terminate() { PostQuitMessage(0); }


END_NAMESPACE(Win32)

END_NAMESPACE(WndDesign)