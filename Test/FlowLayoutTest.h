#include "WndDesign/window/Global.h"
#include "WndDesign/widget/TitleBarFrame.h"
#include "WndDesign/widget/ScrollBox.h"
#include "WndDesign/layout/FlowLayout.h"
#include "WndDesign/control/Button.h"
#include "WndDesign/control/EditBox.h"
#include "WndDesign/frame/MaxFrame.h"
#include "WndDesign/frame/ClipFrame.h"
#include "WndDesign/frame/BorderFrame.h"
#include "WndDesign/frame/PaddingFrame.h"

#include <cmath>
#include <ctime>


using namespace WndDesign;


struct MainFrameStyle : DesktopFrameStyle {
	MainFrameStyle() {
		width.normal(800px).max(100pct);
		height.normal(500px).max(100pct);
		position.setHorizontalCenter().setVerticalCenter();
		border.width(5).color(Color::Violet);
		title.assign(L"FlowLayoutTest");
	}
};


class MyFlowLayout : public FlowLayout {
public:
	MyFlowLayout() : FlowLayout(25, 10, 5) { AppendChild(new AddButton); }
private:
	virtual void OnDraw(FigureQueue& figure_queue, Rect draw_region) override {
		figure_queue.add(draw_region.point, new Rectangle(draw_region.size, Color::White));
		FlowLayout::OnDraw(figure_queue, draw_region);
	}
private:
	class AddButton : public Button<Auto, Assigned> {
	public:
		AddButton() : Button(60) {}
	private:
		MyFlowLayout& GetFlowLayout() const { return static_cast<MyFlowLayout&>(GetParent()); }
	private:
		struct EditBoxStyle : public EditBox::Style {
			EditBoxStyle() {
				paragraph.line_height(90pct);
				font.family(L"Segoe UI").size(18);
			}
		};
		std::wstring NextName() {
			static const wchar* names[26] = { L"Alice", L"Bob", L"Carol", L"Dave", L"Eve", L"Francis", L"Grace", 
				L"Hans", L"Isabella", L"Jason", L"Kate", L"Louis", L"Margaret", L"Nathan", L"Olivia", L"Paul", L"Queen", 
				L"Richard", L"Susan", L"Thomas", L"Uma", L"Vivian", L"Winnie", L"Xander", L"Yasmine", L"Zach" };
			static uint next = 0;
			return next >= 26 ? L"You Win" : names[next++];
		}
		Color GetRandomColor() {
			static bool seeded = []() { srand((uint)time(nullptr)); return true; }();
			Color color = 0; color.red = rand(); color.green = rand(); color.blue = rand(); return color;
		}
	private:
		virtual void OnClick() override {
			GetFlowLayout().AppendChild(
				new BorderFrame<Auto, Assigned>{
					3, 12, GetRandomColor(),
					new PaddingFrame<Auto, Assigned>{
						Margin(5, 0),
						new ClipFrame<Auto, Assigned>{
							new MaxFrame{
								length_max,
								new EditBox(EditBoxStyle(), NextName())
							}
						}
					}
				}
			);
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