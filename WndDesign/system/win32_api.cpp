#include "win32_api.h"
#include "../message/message.h"
#include "../frame/DesktopWndFrame.h"

#include <Windows.h>
#include <windowsx.h>


BEGIN_NAMESPACE(WndDesign)

BEGIN_NAMESPACE(Anonymous)

inline const RECT Rect2RECT(const Rect& rect) {
    return { rect.left(), rect.top(), rect.right(), rect.bottom() };
}

inline const Rect RECT2Rect(const RECT& rect) {
    return Rect(rect.left, rect.top, (uint)(rect.right - rect.left), (uint)(rect.bottom - rect.top));
}

inline bool IsMouseMsg(UINT msg) { return WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST; }
inline bool IsKeyboardMsg(UINT msg) { return WM_KEYFIRST <= msg && msg <= WM_KEYLAST; }


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    ref_ptr<DesktopWndFrame> frame = reinterpret_cast<ref_ptr<DesktopWndFrame>>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (frame == nullptr) { goto FrameIrrelevantMessages; }

    // mouse message
    if (IsMouseMsg(msg)) {
        MouseMsg mouse_msg;
        mouse_msg.point = Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        mouse_msg._key_state = GET_KEYSTATE_WPARAM(wParam);;
        mouse_msg.wheel_delta = GET_WHEEL_DELTA_WPARAM(wParam);
        switch (msg) {
        case WM_MOUSEMOVE:
            mouse_msg.type = MouseMsg::Move;
            // Track mouse message for mouse leave notification.
            if (mouse_tracked_frame == nullptr) {
                TRACKMOUSEEVENT track_mouse_event;
                track_mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
                track_mouse_event.dwFlags = TME_LEAVE;
                track_mouse_event.hwndTrack = hWnd;
                track_mouse_event.dwHoverTime = HOVER_DEFAULT;
                TrackMouseEvent(&track_mouse_event);
                mouse_tracked_frame = frame;
                frame->OnNotifyMsg(NotifyMsg::MouseEnter);
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
        default: return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        frame->OnMouseMsg(mouse_msg);
        return 0;
    }

    // keyboard message
    if (IsKeyboardMsg(msg)) {
        KeyMsg key_msg;
        key_msg.key = static_cast<Key>(wParam);
        key_msg.ch = static_cast<wchar>(wParam);
        switch (msg) {
        case WM_KEYDOWN: key_msg.type = KeyMsg::KeyDown; break;
        case WM_KEYUP: key_msg.type = KeyMsg::KeyUp; break;
        case WM_CHAR: key_msg.type = KeyMsg::Char; break;
        default: return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        frame->OnKeyMsg(key_msg);
        return 0;
    }

    {
        // ime message
        ImeInput& ime = ImeInput::Get();
        switch (msg) {
        case WM_IME_SETCONTEXT:
            ime.CreateImeWindow(hWnd);
            ime.CleanupComposition(hWnd);
            ime.SetImeWindowStyle(hWnd, msg, wParam, lParam);
            break;
        case WM_INPUTLANGCHANGE:
            ime.UpdateInputLanguage();
            break;
        case WM_IME_STARTCOMPOSITION:
            ime.CreateImeWindow(hWnd);
            ime.ResetComposition();
            frame->ReceiveMessage(Msg::ImeCompositionBegin, nullmsg);
            break;
        case WM_IME_COMPOSITION:
            ime.UpdateImeWindow(hWnd);
            if (ime.UpdateComposition(hWnd, lParam)) {
                ImeCompositionMsg ime_composition_msg(ime.GetComposition());
                frame->ReceiveMessage(Msg::ImeComposition, ime_composition_msg);
            }
            ime.UpdateResult(hWnd, lParam);
            break;
        case WM_IME_ENDCOMPOSITION:
        {
            ImeCompositionMsg ime_composition_msg(ime.GetResult());
            frame->ReceiveMessage(Msg::ImeCompositionEnd, ime_composition_msg);
        }
        ime.ResetComposition();
        ime.DestroyImeWindow(hWnd);
        break;

        // region message
        case WM_GETMINMAXINFO:
        {
            if (frame == nullptr) { break; }
            MINMAXINFO* min_max_info = reinterpret_cast<MINMAXINFO*>(lParam);
            auto [min_size, max_size] = frame->GetMinMaxSize();
            min_max_info->ptMaxPosition = { 0,0 };
            min_max_info->ptMaxSize = { static_cast<LONG>(max_size.width), static_cast<LONG>(max_size.height) };
            min_max_info->ptMinTrackSize = { static_cast<LONG>(min_size.width), static_cast<LONG>(min_size.height) };
            min_max_info->ptMaxTrackSize = { static_cast<LONG>(max_size.width), static_cast<LONG>(max_size.height) };
        }break;
        case WM_WINDOWPOSCHANGING:
            break;
        case WM_WINDOWPOSCHANGED:
        {
            WINDOWPOS* position = reinterpret_cast<WINDOWPOS*>(lParam);
            if ((position->flags & SWP_NOSIZE) && (position->flags & SWP_NOMOVE)) { break; }  // Filter out other messages.
            Rect rect(Point(position->x, position->y), Size(static_cast<uint>(position->cx), static_cast<uint>(position->cy)));
            frame->SetRegion(rect); CommitQueue();
        }break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            frame->Draw(RECT2Rect(ps.rcPaint));
            EndPaint(hWnd, &ps);
        }break;

        case WM_MOUSELEAVE: frame->OnNotifyMsg(NotifyMsg::MouseLeave); break;
        case WM_KILLFOCUS: frame->OnNotifyMsg(NotifyMsg::LoseFocus); break;

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
            switch (LOWORD(wParam)) {
            case SB_LINEUP: case SB_PAGEUP:wheel_delta = WHEEL_DELTA; break;
            case SB_LINEDOWN:case SB_PAGEDOWN:wheel_delta = -WHEEL_DELTA; break;
            default: return 0;
            }
            return WndProc(
                hWnd, msg == WM_HSCROLL ? WM_MOUSEHWHEEL : WM_MOUSEWHEEL,
                (wheel_delta << 16) | key_state, ((short)cursor_position.y << 16) | (short)cursor_position.x
            );
        }
        default: goto FrameIrrelevantMessages;
        }
        return 0;
    }

    //// frame irrelevant message ////
FrameIrrelevantMessages:
    switch (msg) {
    case WM_CREATE: break;
    case WM_DESTROY: if (frame != nullptr) { frame->OnDestroy(); }  break;

        // Intercept all non-client messages.
    case WM_NCCALCSIZE: break;  // Process the message to set client region the same as the window region.
    case WM_NCACTIVATE: return TRUE;  // Do not draw the nonclient area.
    case WM_NCHITTEST: return HTCLIENT;  // There's no non-client region.
    //case WM_NCCREATE: return TRUE; // Leave the message alone, or title won't get displayed.
    //case WM_NCDESTROY: break; // It doesn't matter.
    case WM_NCPAINT: break;

    case WM_ERASEBKGND: break;  // Intercept WM_ERASEBKGND from DefWindowProc(), else there may be problem using driect2d.

    default: return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;  // The message is handled.
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


HANDLE CreateWnd(Rect region, const std::wstring& title) {
    RegisterWndClass();
    HWND hWnd = CreateWindowExW(WS_EX_NOREDIRECTIONBITMAP, wnd_class_name, title.c_str(),
                                WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_HSCROLL | WS_VSCROLL,
                                region.point.x, region.point.y, region.size.width, region.size.height,
                                NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) { throw std::runtime_error("create window error"); }
    return hWnd;
}

void DestroyWnd(HANDLE hWnd) {
    DestroyWindow((HWND)hWnd);
}

void SetWndUserData(HANDLE hWnd, void* data) {
    SetWindowLongPtrW((HWND)hWnd, GWLP_USERDATA, (LONG_PTR)data);
}

void SetWndTitle(HANDLE hWnd, const std::wstring& title) {
    SetWindowTextW((HWND)hWnd, title.c_str());
}

void SetWndRegion(HANDLE hWnd, Rect region) {
    MoveWindow((HWND)hWnd, region.point.x, region.point.y, region.size.width, region.size.height, false);
}

void InvalidateWndRegion(HANDLE hWnd, Rect region) {
    InvalidateRect(hWnd, , false);
}

void SetCapture(HANDLE hWnd) {
    ::SetCapture((HWND)hWnd);
}

void ReleaseCapture() {
    ::ReleaseCapture();
}

void SetFocus(HANDLE hWnd) {
    ::SetFocus((HWND)hWnd);
}

int MessageLoop() {
    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}

void Terminate() {
    PostQuitMessage(0);
}


END_NAMESPACE(Win32)

END_NAMESPACE(WndDesign)