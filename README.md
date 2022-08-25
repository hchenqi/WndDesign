# WndDesign

A C++ GUI library

## Quick Start

* Install Visual Studio 2022, check **Desktop development with C++**.

* Clone `WndDesign` source code.

* Click open WndDesign.sln, set `Test` as startup project, build, and run.

## Code Structure

### Folder Hierarchy

There are two projects under the `WndDesign` solution:
* `WndDesign`: main source code, builds to a static library `WndDesign.lib`.
* `Test`: test examples, builds to an executable application `Test.exe`.

#### WndDesign

Folder-level dependencies are roughly shown in the graph to help one form an overall impression, although precise dependencies should certainly be more complex.

![](docs/dependency.png)

The main contents of each folder are also summarized below.

##### common

Defines some basic macros, namespaces, and types.

##### geometry

Defines basic 2D geometry objects and their operations, including `Point`, `Vector`, `Size` and `Rect`. 

##### figure

Defines some basic shapes including `Line`, `Rectangle`, `RoundedRectangle`, `Ellipse`, and other figures like `TextBlock`, `Image` and `Layer`.

##### style

Defines styles that are used by `TextBlock` and other components.

##### message

Defines `MouseMsg`, `KeyMsg`, `NotifyMsg` and `Timer` for message handling.

##### system

Includes `Win32`, `Direct2D` and `Direct3D` APIs for window rendering and message receiving.

##### window

Defines the `WndObject` base class and `Desktop` root window object.

##### control

Defines some basic control components such as `Button`, `Textbox`, `ImageBox`, `Scrollbar`, etc.

##### frame

Defines some basic frame components like `BorderFrame`, `PaddingFrame`, `ScrollFrame`, and so on.

##### layout

Defines some layout components like `ListLayout`, `SplitLayout`, and `OverlapLayout`.

##### wrapper

Defines some window decorators like `SolidColorBackground`.

##### widget

Provides some pre-defined complex window components that are combination of those basic window components. Currently includes `ScrollBox` and `TitleBarFrame`.

#### Test

Defines some {Test}.h header files that serve as both test cases and develop examples.

### Class Hierarchy

`WndObject` is the base class for all windows including Controls, Frames, Layouts and `Desktop`.

## Concepts

### Window Hierarchy

Window, or `WndObject`, is a basic unit that draws figures and handle messages.

A window may have none, one, or multiple child windows, and each window may only have one parent window.

All window objects, along with their child windows and parent windows, form a *window tree*, which is just like the DOM tree in web browsers.

A runtime picture of the test example `FlowLayoutTest` is shown below along with its main window hierarchy components:

![](docs/example-picture.png)

![](docs/example-hierarchy.png)

#### Control, Frame and Layout

Controls, Frames and Layouts all derive from `WndObject` base class and play different roles in the window tree.

![](docs/component.png)

Controls are `WndObject` that have no child window, so they are leaf nodes in the *window tree*, and serve as the most basic components. They are often used to handle user inputs or display simple figures, like `Button`, `TextBox`, `EditBox`, `ImageBox`, and `Placeholder`. A `Placeholder` only occupies a certain region and doesn't draw anything.

Frames are `WndObject` that may have only one child window, and are often used to decorate a window with border, padding, or change the window's resizing or drawing behaviour. Frames include `BorderFrame`, `PaddingFrame`, `ClipFrame`, `FixedFrame`, `ScaleFrame`, etc. All Frames inherits `WndFrame` which is derived from `WndObject` and has implemented all basic virtual functions. A window wrapped only with `WndFrame` behaves the same way as the window itself.

Layouts are `WndObject` that may contain multiple child windows, which are used to display complex data structures. For example, `SplitLayout` represent a pair of windows that may be placed in horizontal or vertical direction, `ListLayout` or `FlowLayout` a list of windows. `OverlapLayout`'s child windows are placed in order and may overlap on each other.

#### Wrapper

Wrappers are decorator templates that inherite a window and override some of its virtual functions. For example, `SolidColorBackground` will override the window's `OnDraw` callback function to fill a solid color in its background before the window's content is drawn.

![](docs/wrapper.png)

Both Frames and Wrappers can be used to decorate a window. A Frame and its child window are distinct window objects in the window tree, while a Wrapper and its wrapped window are exactly the same window object.

#### Desktop

`Desktop` is the root window object who has no parent window. `Desktop` globally manages all windows and provides system interfaces.

![](docs/Desktop.png)

`DesktopFrame` is the direct child window of `Desktop` that each owns a win32 `HWND` resource.

`DesktopFrame` actually derives from `ScaleFrame` and contains a `BorderFrame` which then wraps the child window. `ScaleFrame` applies a scale transform to its descendent, which is used by `DesktopFrame` to handle DPI change. Previously `DesktopFrame` only derives from `BorderFrame` to add a border to the child window.

### Redraw

Redraw happens when a window is created or its displaying contents have changed. A rectangle region of the window is invalidated and propogated to its parent windows til `DesktopWndFrame`. 

All figures inherits `Figure` base class, and are rendered to a `RenderTarget` in a callback virtual function `DrawOn()`. 

Rendering is finally implemented by Direct2D, a Windows built-in library. A `RenderTarget` is a kind of Direct2D object to create resources and perform actual drawing operations.

#### Geometry

2D geometric operations such as point translation, vector addition, rectangle enlarging, size scaling, are frequently used throughout the project.

##### Point

##### Vector

##### Size

##### Rectangle

##### Region

#### Figure

##### Shape

##### TextBlock

##### Image

#### FigureQueue

#### Layer

### Reflow

Reflow happens when a window's size is changed due to mouse dragging, content editing, or parent window's resizing. The layout of the window may be recalculated and the content may be further redrawed.

Windows react differently to resize events. Some windows' size may change exactly according to mouse's current drag position and reorgnizes its content, like `FlowLayout`, while some windows' size may remain the same, and the content is clipped out of scene or a scrollbar is displayed.

The rather complex example `FlowLayoutTest` shows different reactions to resize events. The 

![]()

#### LayoutType

`LayoutType` is used to indicate how a window's width or height will change relative to parent window's width or height. It is just a compile-time contract to help ensure child window and parent window's reflow behaviours are consistent. Actual size calculation still needs to be implemented.

There are three kinds of layout types for both horizontal and virtical dimensions: `Assigned`, `Auto` and `Relative`.

A window's size may be dependent on its parent window's size or its content's size. For example, when you drag the side of a text editor to make it thinner, the text block's width is shortened exactly the same length as the window's width but its height is extended due to the reflow of the text.

There are two functions designed to handle all reflow conditions: `OnSizeRefUpdate()` and `OnChildSizeChange()`. A window receives `size_ref` from its parent window. This information, along with the window's current style, content and child windows, are all needed for its size calculation. After its current size is determined and returned to its parent window, the parent window's size will be finally determined.

I will only talk about the honrizontal dimension `width` in below sections for simplicity. It is exactly the same for `height`.

##### Assigned

If the width of a window is `Assigned` by its parent window, its actual width is expected to be the same as `size_ref.width` that parent window provides.

If both width and height of a window are `Assigned`, the window will be expected to never call `SizeChanged()` and its parent window will leave `OnChildSizeChange()` empty.

##### Auto

##### Relative



### Message Handling

#### Mouse Messages

#### Keyboard Messages

#### Other Notifications

#### Timer

### Desktop

## Develop Guide

Refer to CogNote repository.
