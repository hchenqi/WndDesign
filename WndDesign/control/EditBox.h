#pragma once

#include "TextBox.h"
#include "../message/timer.h"
#include "../message/ime.h"
#include "../message/mouse_tracker.h"
#include "../common/unicode_helper.h"


BEGIN_NAMESPACE(WndDesign)


class EditBox : public TextBox, private ImeMsgHandler {
private:
	struct EditBoxStyle {
		struct EditStyle {
		public:
			Color _selection_color = Color(Color::DimGray, 0x7f);
			Color _caret_color = Color::DimGray;
			bool _disable_edit = false;  // can only select and copy
		public:
			constexpr EditStyle& selection_color(Color selection_color) { _selection_color = selection_color; return *this; }
			constexpr EditStyle& caret_color(Color caret_color) { _caret_color = caret_color; return *this; }
			constexpr EditStyle& disable_edit() { _disable_edit = true; return *this; }
		}edit;
	};
public:
	struct Style : TextBox::Style, EditBoxStyle {};

public:
	EditBox(Style style, std::wstring text = L"") : TextBox(style, text), ImeMsgHandler(this), mouse_tracker(*this) { 
		TextUpdated(); if (style.edit._disable_edit) { ImeDisable(); }
	}
	~EditBox() {}

	// style
private:
	EditBoxStyle style;
private:
	bool IsEditDisabled() const { return style.edit._disable_edit; }

	// text block
public:
	using HitTestInfo = TextBlockHitTestInfo;
private:
	WordBreakIterator word_break_iterator;
private:
	uint GetCharacterLength(uint text_position) { return GetUTF16CharLength(text[text_position]); }
	void TextUpdated() { TextBox::TextUpdated(); word_break_iterator.SetText(text); }
public:
	void SetText(std::wstring str) { text.assign(std::move(str)); TextUpdated(); }
	void InsertText(uint pos, wchar ch) { text.insert(pos, 1, ch); TextUpdated(); }
	void InsertText(uint pos, const std::wstring& str) { text.insert(pos, str); TextUpdated(); }
	void ReplaceText(uint begin, uint length, wchar ch) { text.replace(begin, length, 1, ch); TextUpdated(); }
	void ReplaceText(uint begin, uint length, const std::wstring& str) { text.replace(begin, length, str); TextUpdated(); }
	void DeleteText(uint begin, uint length) { text.erase(begin, length); TextUpdated(); }

	// layout and paint
protected:
	virtual const Size OnSizeRefUpdate(Size size_ref) override;
protected:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) const;

	// caret
private:
	static constexpr ushort caret_blink_period = 500;  // 500ms
	static constexpr ushort caret_blink_expire_time = 20000;  // 20s
	enum class CaretState : ushort { Hide, Show, BlinkHide, BlinkShow };
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
	static constexpr uint caret_width = 1;
	enum class CaretMoveDirection { Left, Right, Up, Down, Home, End };
private:
	uint caret_text_position = 0;
	Rect caret_region = region_empty;
private:
	void UpdateCaretRegion(const HitTestInfo& info);
private:
	void SetCaret(Point mouse_down_position);
	void SetCaret(uint text_position, bool is_trailing_hit);
	void MoveCaret(CaretMoveDirection direction);

	// selection
private:
	uint mouse_down_text_position = 0;
	uint selection_begin = 0;
	uint selection_end = 0;
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
	void Insert(const std::wstring& str);
	void Delete(bool is_backspace);

	// ime input
private:
	uint ime_composition_begin = 0;
	uint ime_composition_end = 0;
private:
	virtual void OnImeCompositionBegin() override;
	virtual void OnImeComposition(std::wstring str) override;
	virtual void OnImeCompositionEnd() override {}

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
	virtual void OnMouseMsg(MouseMsg msg);
	virtual void OnKeyMsg(KeyMsg msg);
	virtual void OnNotifyMsg(NotifyMsg msg);
};


END_NAMESPACE(WndDesign)