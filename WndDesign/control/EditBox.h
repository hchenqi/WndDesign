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
			bool _disabled = false;  // can only select and copy if disabled
		public:
			constexpr EditStyle& selection_color(Color selection_color) { _selection_color = selection_color; return *this; }
			constexpr EditStyle& caret_color(Color caret_color) { _caret_color = caret_color; return *this; }
			constexpr EditStyle& disable() { _disabled = true; return *this; }
		}edit;
	};
public:
	struct Style : TextBox::Style, EditBoxStyle {};

public:
	EditBox(Style style, std::wstring text = L"");
	~EditBox() {}

	// style
private:
	EditBoxStyle style;
private:
	bool IsEditDisabled() const { return style.edit._disabled; }

	// text
protected:
	using HitTestInfo = TextBlock::HitTestInfo;
private:
	mutable WordBreakIterator word_break_iterator;
private:
	size_t GetCharacterLength(size_t position) const;
	TextRange GetWordRange(size_t position) const;
	TextRange GetParagraphRange(size_t position) const;
	TextRange GetEntireRange() const { return TextRange(0, text.length()); }
protected:
	virtual void OnTextUpdate() override;

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
private:
	void HideCaret();
	void StartBlinkingCaret();
	void BlinkCaret();

	// caret position
private:
	static constexpr float caret_width = 1.0f;
	enum class CaretMoveDirection { Left, Right, Up, Down, Home, End };
private:
	size_t caret_position = 0;
	Rect caret_region = region_empty;
private:
	void UpdateCaret(const HitTestInfo& info);
	void UpdateCaret(size_t position) { UpdateCaret(text_block.HitTestPosition(position)); }
private:
	void SetCaret(const HitTestInfo& info);
	void SetCaret(Point point) { SetCaret(text_block.HitTestPoint(point)); }
	void SetCaret(size_t position) { SetCaret(text_block.HitTestPosition(position)); }
	void MoveCaret(CaretMoveDirection direction);

	// selection
private:
	enum class SelectionMode { Character, Word, Paragraph };
private:
	SelectionMode selection_mode = SelectionMode::Character;
	TextRange selection_initial_range;
private:
	TextRange selection_range;
	std::vector<HitTestInfo> selection_info;
	Rect selection_region;
private:
	bool HasSelection() const { return !selection_range.IsEmpty(); }
	void UpdateSelection(TextRange range);
private:
	void SelectWord();
	void SelectParagraph();
	void SelectAll() { UpdateSelection(GetEntireRange()); }
private:
	void DoSelection(const HitTestInfo& info);
	void DoSelection(Point current_point) { DoSelection(text_block.HitTestPoint(current_point)); }
	void DoSelection(size_t current_position) { DoSelection(text_block.HitTestPosition(current_position)); }

	// keyboard input
private:
	void Insert(wchar ch);
	void Insert(const std::wstring& str);
	void Delete(bool is_backspace);

	// ime input
private:
	TextRange ime_composition_range;
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