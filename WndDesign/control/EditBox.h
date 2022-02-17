#pragma once

#include "TextBox.h"
#include "../common/unicode_helper.h"
#include "../message/timer.h"
#include "../message/ime.h"
#include "../message/mouse_tracker.h"


BEGIN_NAMESPACE(WndDesign)


class EditBox : public TextBox {
private:
	struct EditBoxStyle {
		struct EditStyle {
		public:
			Color _selection_color = Color(Color::DimGray, 0x7f);
			Color _caret_color = Color::DimGray;
			bool _disabled = false;  // can only select and copy
		public:
			constexpr EditStyle& selection_color(Color selection_color) { _selection_color = selection_color; return *this; }
			constexpr EditStyle& caret_color(Color caret_color) { _caret_color = caret_color; return *this; }
			constexpr EditStyle& disable() { _disabled = true; return *this; }
		}edit;
	};
public:
	struct Style : TextBox::Style, EditBoxStyle {};

public:
	EditBox(Style style, std::wstring text = L"") : TextBox(style, text), style(style) {
		cursor = Cursor::Text;
		style.edit._disabled ? ime.Disable(*this) : ime.Enable(*this);
		TextUpdated();
	}
	~EditBox() {}

	// style
private:
	EditBoxStyle style;
private:
	bool IsEditDisabled() const { return style.edit._disabled; }

	// text
public:
	using HitTestInfo = TextBlockHitTestInfo;
private:
	WordBreakIterator word_break_iterator;
private:
	size_t GetCharacterLength(size_t text_position) { return GetUTF16CharLength(text[text_position]); }
	void TextUpdated() { word_break_iterator.SetText(text); }
public:
	void SetText(std::wstring str) { TextBox::SetText(str); TextUpdated(); }
	void InsertText(size_t pos, wchar ch) { TextBox::InsertText(pos, ch); TextUpdated(); }
	void InsertText(size_t pos, std::wstring str) { TextBox::InsertText(pos, str); TextUpdated(); }
	void ReplaceText(size_t begin, size_t length, wchar ch) { TextBox::ReplaceText(begin, length, ch); TextUpdated(); }
	void ReplaceText(size_t begin, size_t length, std::wstring str) { TextBox::ReplaceText(begin, length, str); TextUpdated(); }
	void DeleteText(size_t begin, size_t length) { TextBox::DeleteText(begin, length); TextUpdated(); }

	// layout
protected:
	virtual Size OnSizeRefUpdate(Size size_ref) override;

	// paint
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override;

	// caret
private:
	static constexpr ushort caret_blink_period = 500;  // 500ms
	static constexpr ushort caret_blink_expire_time = 20000;  // 20s
	enum class CaretState : ushort { Hide, Show, BlinkShow, BlinkHide };
private:
	Timer caret_timer = Timer(std::bind(&EditBox::BlinkCaret, this));
	CaretState caret_state = CaretState::Hide;
	ushort caret_blink_time = 0;
private:
	bool IsCaretVisible() const { return caret_state == CaretState::Show || caret_state == CaretState::BlinkShow; }
	void RedrawCaretRegion() { Redraw(caret_region); }
private:
	void HideCaret();
	void StartBlinkingCaret();
	void BlinkCaret();

	// caret position
private:
	static constexpr float caret_width = 1.0f;
	enum class CaretMoveDirection { Left, Right, Up, Down, Home, End };
private:
	size_t caret_text_position = 0;
	Rect caret_region = region_empty;
private:
	void UpdateCaretRegion(const HitTestInfo& info);
private:
	void SetCaret(HitTestInfo info);
	void SetCaret(Point point) { SetCaret(text_block.HitTestPoint(point)); }
	void SetCaret(size_t text_position) { SetCaret(text_block.HitTestTextPosition(text_position)); }
	void MoveCaret(CaretMoveDirection direction);

	// selection
private:
	size_t mouse_down_text_position = 0;
	size_t selection_begin = 0;
	size_t selection_end = 0;
	std::vector<HitTestInfo> selection_info;
	Rect selection_region_union;
private:
	void UpdateSelectionRegion();
	void RedrawSelectionRegion() { Redraw(selection_region_union); }
private:
	bool HasSelection() const { return selection_end > selection_begin; }
	void DoSelection(Point mouse_move_position);
	void SelectWord();
	void SelectParagraph();
	void SelectAll();
	void ClearSelection();

	// keyboard input
private:
	void Insert(wchar ch);
	void Insert(std::wstring str);
	void Delete(bool is_backspace);

	// ime input
private:
	size_t ime_composition_begin = 0;
	size_t ime_composition_end = 0;
private:
	void OnImeBegin();
	void OnImeString();
	void OnImeEnd();

	// clipboard
private:
	void Cut();
	void Copy();
	void Paste();

	// message
private:
	MouseTracker mouse_tracker;
	bool is_ctrl_down = false;
	bool is_shift_down = false;
protected:
	virtual void OnMouseMsg(MouseMsg msg) override;
	virtual void OnKeyMsg(KeyMsg msg) override;
	virtual void OnNotifyMsg(NotifyMsg msg) override;
};


END_NAMESPACE(WndDesign)