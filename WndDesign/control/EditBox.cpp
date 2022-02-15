#include "EditBox.h"
#include "../figure/shape.h"
#include "../system/clipboard.h"


BEGIN_NAMESPACE(WndDesign)


Size EditBox::OnSizeRefUpdate(Size size_ref) {
	Size size = TextBox::OnSizeRefUpdate(size_ref);
	UpdateCaretRegion(text_block.HitTestTextPosition(caret_text_position));
	UpdateSelectionRegion();
	return size;
}

void EditBox::OnDraw(FigureQueue& figure_queue, Rect draw_region) {
	TextBox::OnDraw(figure_queue, draw_region);
	if (IsCaretVisible() && !caret_region.Intersect(draw_region).IsEmpty()) {
		figure_queue.add(caret_region.point, new Rectangle(caret_region.size, style.edit._caret_color));
	}
	if (HasSelection()) {
		for (auto& it : selection_info) {
			auto& region = it.geometry_region; if (region.Intersect(draw_region).IsEmpty()) { continue; }
			figure_queue.add(region.point, new Rectangle(region.size, style.edit._selection_color));
		}
	}
}

void EditBox::HideCaret() {
	if (caret_state != CaretState::Hide) {
		caret_state = CaretState::Hide;
		RedrawCaretRegion();
	}
}

void EditBox::StartBlinkingCaret() {
	if (caret_state != CaretState::Hide) {
		if (!caret_timer.IsSet()) {
			caret_timer.Set(caret_blink_period);
		}
		caret_blink_time = 0;
	}
}

void EditBox::BlinkCaret() {
	caret_blink_time += caret_blink_period;
	if (caret_blink_time >= caret_blink_expire_time) {
		caret_state = CaretState::Show;
		caret_timer.Stop();
		return;
	}
	switch (caret_state) {
	case CaretState::Hide: caret_timer.Stop(); return;
	case CaretState::Show:
	case CaretState::BlinkShow: caret_state = CaretState::BlinkHide; break;
	case CaretState::BlinkHide: caret_state = CaretState::BlinkShow; break;
	}
	RedrawCaretRegion();
}

void EditBox::UpdateCaretRegion(const HitTestInfo& info) {
	RedrawCaretRegion();
	caret_text_position = info.text_position;
	caret_region.point = info.geometry_region.point;
	caret_region.size = Size(caret_width, info.geometry_region.size.height);
	if (info.is_trailing_hit) {
		caret_text_position += info.text_length;
		caret_region.point.x += info.geometry_region.size.width;
	}
	RedrawCaretRegion();
}

void EditBox::SetCaret(HitTestInfo info) {
	SetFocus();
	UpdateCaretRegion(info); caret_state = CaretState::Show;
	ClearSelection();
}

void EditBox::MoveCaret(CaretMoveDirection direction) {
	switch (direction) {
	case CaretMoveDirection::Left:
		if (HasSelection()) {
			SetCaret(selection_begin);
		} else {
			if (caret_text_position > 0) {
				SetCaret(caret_text_position - 1);
			}
		}
		break;
	case CaretMoveDirection::Right:
		if (HasSelection()) {
			SetCaret(selection_end);
		} else {
			SetCaret(caret_text_position + GetCharacterLength(caret_text_position));
		}
		break;
	case CaretMoveDirection::Up:
		SetCaret(caret_region.Center() - Vector(0, caret_region.size.height));
		break;
	case CaretMoveDirection::Down:
		SetCaret(caret_region.Center() + Vector(0, caret_region.size.height));
		break;
	case CaretMoveDirection::Home:
		SetCaret(Point(0, caret_region.Center().y));
		break;
	case CaretMoveDirection::End:
		SetCaret(Point(position_max, caret_region.Center().y));
		break;
	}
}

void EditBox::UpdateSelectionRegion() {
	selection_info = text_block.HitTestTextRange(selection_begin, selection_end - selection_begin);
	RedrawSelectionRegion();
	selection_region_union = region_empty;
	for (auto& it : selection_info) {
		selection_region_union = selection_region_union.Union(it.geometry_region);
	}
	RedrawSelectionRegion();
}

void EditBox::DoSelection(Point mouse_move_position) {
	HitTestInfo info = text_block.HitTestPoint(mouse_move_position);
	UpdateCaretRegion(info);
	selection_begin = mouse_down_text_position;
	selection_end = caret_text_position;
	if (selection_begin == selection_end) { ClearSelection(); return; }
	if (selection_end < selection_begin) { std::swap(selection_begin, selection_end); }
	UpdateSelectionRegion(); HideCaret();
}

void EditBox::SelectWord() {
	if (caret_text_position >= text.length()) { return; }
	TextRange word_range = word_break_iterator.Seek(caret_text_position);
	selection_begin = word_range.left(); selection_end = word_range.right();
	UpdateSelectionRegion(); HideCaret();
}

void EditBox::SelectParagraph() {
	size_t length = text.length();
	selection_begin = caret_text_position - 1; selection_end = caret_text_position;
	while (selection_begin < length && text[selection_begin] != L'\n') { selection_begin--; }
	while (selection_end < length && text[selection_end] != L'\n') { selection_end++; }
	selection_begin++; selection_end++;
	UpdateSelectionRegion(); HideCaret();
}

void EditBox::SelectAll() {
	selection_begin = 0;
	selection_end = text.length();
	UpdateSelectionRegion(); HideCaret();
}

void EditBox::ClearSelection() {
	selection_begin = selection_end = 0;
	selection_info.clear();
	RedrawSelectionRegion();
	selection_region_union = region_empty;
}

void EditBox::Insert(wchar ch) {
	if (IsEditDisabled()) { return; }
	if (HasSelection()) {
		ReplaceText(selection_begin, selection_end - selection_begin, ch);
		SetCaret(selection_begin + 1);
	} else {
		InsertText(caret_text_position, ch);
		SetCaret(caret_text_position + 1);
	}
}

void EditBox::Insert(std::wstring str) {
	if (IsEditDisabled()) { return; }
	if (HasSelection()) {
		ReplaceText(selection_begin, selection_end - selection_begin, str);
		SetCaret(selection_begin + str.length());
	} else {
		InsertText(caret_text_position, str);
		SetCaret(caret_text_position + str.length());
	}
}

void EditBox::Delete(bool is_backspace) {
	if (IsEditDisabled()) { return; }
	if (HasSelection()) {
		DeleteText(selection_begin, selection_end - selection_begin);
		SetCaret(selection_begin);
	} else {
		if (is_backspace) {
			if (caret_text_position == 0) { return; }
			size_t previous_caret_position = caret_text_position;
			SetCaret(caret_text_position - 1);
			DeleteText(caret_text_position, previous_caret_position - caret_text_position);
		} else {
			if (caret_text_position >= text.length()) { return; }
			DeleteText(caret_text_position, GetCharacterLength(caret_text_position));
		}
	}
}

void EditBox::OnImeBegin() {
	if (IsEditDisabled()) { return; }
	Point ime_position;
	if (HasSelection()) {
		ime_composition_begin = selection_begin;
		ime_composition_end = selection_end;
		ime_position = selection_info.front().geometry_region.RightBottom();
	} else {
		ime_composition_begin = caret_text_position;
		ime_composition_end = ime_composition_begin;
		ime_position = caret_region.RightBottom();
	}
	ime.SetPosition(*this, ime_position);
}

void EditBox::OnImeString(std::wstring str) {
	if (IsEditDisabled()) { return; }
	ReplaceText(ime_composition_begin, ime_composition_end - ime_composition_begin, str);
	ime_composition_end = ime_composition_begin + str.length();
	SetCaret(ime_composition_end);
}

void EditBox::Cut() {
	if (HasSelection()) {
		Copy();
		Delete(false);
	}
}

void EditBox::Copy() {
	if (HasSelection()) {
		SetClipboardData(text.substr(selection_begin, selection_end - selection_begin));
	}
}

void EditBox::Paste() {
	if (IsEditDisabled()) { return; }
	std::wstring str; GetClipboardData(str);
	if (!str.empty()) { Insert(str); }
}

void EditBox::OnMouseMsg(MouseMsg msg) {
	switch (msg.type) {
	case MouseMsg::LeftDown: SetCaret(msg.point); mouse_down_text_position = caret_text_position; SetCapture(); break;
	case MouseMsg::LeftUp: ReleaseCapture(); break;
	}
	switch (mouse_tracker.Track(msg)) {
	case MouseTrackMsg::LeftDoubleClick: SelectWord(); break;
	case MouseTrackMsg::LeftTripleClick: SelectParagraph(); break;
	case MouseTrackMsg::LeftDrag: DoSelection(msg.point); break;
	}
	StartBlinkingCaret();
}

void EditBox::OnKeyMsg(KeyMsg msg) {
	switch (msg.type) {
	case KeyMsg::KeyDown:
		switch (msg.key) {
		case Key::Left: MoveCaret(CaretMoveDirection::Left); break;
		case Key::Right: MoveCaret(CaretMoveDirection::Right); break;
		case Key::Up: MoveCaret(CaretMoveDirection::Up); break;
		case Key::Down: MoveCaret(CaretMoveDirection::Down); break;
		case Key::Home: MoveCaret(CaretMoveDirection::Home); break;
		case Key::End: MoveCaret(CaretMoveDirection::End); break;

		case Key::Enter: Insert(L'\n'); break;
		case Key::Tab: Insert(L'\t'); break;

		case Key::Backspace: Delete(true); break;
		case Key::Delete: Delete(false); break;

		case Key::Ctrl: is_ctrl_down = true; break;
		case Key::Shift: is_shift_down = true; break;

		case CharKey('A'): if (is_ctrl_down) { SelectAll(); } break;
		case CharKey('X'): if (is_ctrl_down) { Cut(); } break;
		case CharKey('C'): if (is_ctrl_down) { Copy(); } break;
		case CharKey('V'): if (is_ctrl_down) { Paste(); } break;
		}
		break;
	case KeyMsg::KeyUp:
		switch (msg.key) {
		case Key::Ctrl: is_ctrl_down = false; break;
		case Key::Shift: is_shift_down = false; break;
		}
		break;
	case KeyMsg::Char:
		if (is_ctrl_down) { break; }
		if (!iswcntrl(msg.ch)) { Insert(msg.ch); };
		break;
	case KeyMsg::ImeBegin:
		OnImeBegin();
		break;
	case KeyMsg::ImeString:
		OnImeString(ime.GetString());
		break;
	}
	StartBlinkingCaret();
}

void EditBox::OnNotifyMsg(NotifyMsg msg) {
	TextBox::OnNotifyMsg(msg);
	switch (msg) {
	case NotifyMsg::LoseFocus: ClearSelection(); HideCaret(); break;
	}
}


END_NAMESPACE(WndDesign)