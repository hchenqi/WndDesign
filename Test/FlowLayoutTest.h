#include "WndDesign/window/Global.h"
#include "WndDesign/widget/TitleBarFrame.h"
#include "WndDesign/widget/ScrollBox.h"
#include "WndDesign/layout/FlowLayout.h"
#include "WndDesign/control/Button.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/frame/MaxFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/wrapper/Background.h"
#include "WndDesign/wrapper/Border.h"
#include "WndDesign/wrapper/Padding.h"

#include <cmath>
#include <ctime>


using namespace WndDesign;


struct MainFrameStyle : TitleBarFrame::Style {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(4).color(Color::Violet);
		title.assign(L"FlowLayoutTest");
		title_bar.background(Color::CadetBlue);
		title_format.font.size(18);
	}
};


class MyFlowLayout : public Decorate<FlowLayout, SolidColorBackground> {
public:
	MyFlowLayout() : Base(25, 10, 5) { AppendChild(new AddButton); }
private:
	class Item : public Decorate<ClipFrame<Auto, Assigned>, Padding, Border> {
	public:
		struct EditBoxStyle : public EditBox::Style {
			EditBoxStyle() {
				paragraph.line_height(90pct);
				font.family(L"Segoe UI").size(18);
			}
		};
	public:
		Item(std::wstring text, Color border_color) :
			Base(
				new MaxFrame{
					length_max,
					new EditBox(EditBoxStyle(), text)
				}
			) {
			border.width(3).radius(12).color(border_color);
			padding = Margin(5, 0);
		}
	};
	class AddButton : public Button<Auto, Assigned> {
	public:
		AddButton() : Button(60) {}
	private:
		MyFlowLayout& GetFlowLayout() const { return static_cast<MyFlowLayout&>(GetParent()); }
	private:
		std::wstring NextName() {
			static const wchar* names[27] = { L"Alice", L"Bob", L"Carol", L"Dave", L"Eve", L"Francis", L"Grace",
				L"Hans", L"Isabella", L"Jason", L"Kate", L"Louis", L"Margaret", L"Nathan", L"Olivia", L"Paul", L"Queen",
				L"Richard", L"Susan", L"Thomas", L"Uma", L"Vivian", L"Winnie", L"Xander", L"Yasmine", L"Zach", L"You Win" };
			static uint next = 0;
			return names[next >= 26 ? next : next++];
		}
		Color GetRandomColor() {
			static bool seeded = []() { srand((uint)time(nullptr)); return true; }();
			Color color = 0; color.red = rand(); color.green = rand(); color.blue = rand(); return color;
		}
	private:
		virtual void OnClick() override {
			GetFlowLayout().AppendChild(new Item(NextName(), GetRandomColor()));
		}
	};
};


int main() {
	global.AddWnd(
		new TitleBarFrame{
			MainFrameStyle(),
			new ScrollBox{
				new MyFlowLayout
			}
		}
	);
	global.MessageLoop();
}