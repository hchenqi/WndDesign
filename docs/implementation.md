# Implementation

## Window Tree

Parent windows, especially Layouts, use their internal data structures to manage child windows. Most 

Each window stores a reference to its parent window, after it is registered by its parent window. Parent window may also store some data to each of its child window for indexing. Bottom up callback functions can be routed.

### Child Window Management

A parent-child relation is established by parent calling `WndObject::RegisterChild(WndObject& child)`.


Each window can only be registered by one parent window. Only after registration can a window be added to the *window tree* to receive messages and display contents. Parent window registers a child window by its reference, `WndObject&`.

Parent window can actually own the child window's resource with a `unique_ptr<WndObject>`. This technique is used across almost all examples, which makes it easier to define window tree.

## Window Layout

### Geometry

All position or length values are stored in `float` to support high DPI display and scale transformation, although they will be eventually converted to integers when rendered as pixels on screen.

### Reflow

Layout calculation logic might be the most interesting and complicated part as it took me the most time to finally find a neat design solution. Re-calculating layout is also called the `reflow` process.

Each window is aware of its own size and its content layout. A window's content includes all its child windows and their sizes and positions, or its raw content if it has no child window, like `TextBox` or `ImageBox`. Child window's position relative to its parent window is always kept by the parent and never exposed to the child window itself.

A window's size may be dependent on its parent window's size or its content size. For example, when you drag the border of a text editor to make it thinner, the text will probably be wrapped to a new line because the width is shortened, but its height is extended and a scrollbar may appear to be able to navigate to the hidden content. A similar behaviour happens on `FlowLayout`, as is shown below in the same `FlowLayoutTest` example:

![](docs/layout-1.gif)

![](docs/layout-2.gif)

Sometimes a window's width and height are both assigned by its parent window, so the parent window already knows its child window's size information. Sometimes parent window only assigns the width of its child, but requires the child to calculate its height and return it back. Sometimes the child window calculates its size independently and returns both width and height to its parent window.

All these reflow conditions are handled by only two virtual functions, `OnSizeRefUpdate()` and `OnChildSizeUpdate()`. `OnSizeRefUpdate()` goes top-down, which is initiated by parent window calling `UpdateChildSizeRef()`, while `OnChildSizeUpdate()` goes bottom-up and is initiated by child window calling `SizeUpdated()`.

A window receives a size reference, or `size_ref` from its parent window when `OnSizeRefUpdate()` is called. This information, along with the window's style, content and child windows' size, are all needed for calculating its current size. After its size is determined, the parent window's size will be finally determined, and the grand-parent window's size, and so on.

If a window's size is recalculated due to its content change, it will notify its parent window on its current size. Parent window's `OnChildSizeUpdate()` will be called and the parent window's size will in turn be recalculated and grand-parent window may be further informed.

The animation below shows the size calculation order of the windows in a *window tree*. A blue node indicates the window's size is old, green means the size is calculating, and orange means the size is calculated. The window first updates all its child windows' size_ref and gets their updated sizes, then calculates its own size, and finally informs its parent window.

![](docs/reflow.gif)

## Drawing


### Figure

All figures inherits `Figure` base class, and are rendered to a `RenderTarget` in a callback virtual function `DrawOn()`. 

Rendering is finally implemented by Direct2D, a Windows built-in library. A `RenderTarget` is a kind of Direct2D object to create resources and perform actual drawing operations.

#### Shape

An `Ellipse` figure is drawn on a `RenderTarget`, whose center is located at `Point(500, 300)` relative to the `RenderTarget`.

#### TextBlock

#### Image

Redraw happens when a window is created or its displaying contents have changed. A rectangle region of the window is invalidated and propogated to its parent windows til `DesktopFrame`. 

### FigureQueue

A `FigureQueue` is a collection of `Figure`s. It will be passed to the window by reference in the `OnDraw()` callback function to collect `Figure`s the window wishes to draw. It can also apply translation or other complex transformations to a group of figures. `FigureQueue` is finally drawn on `Layer`.

#### Offset

#### Group

A figure group has a 

### Layer

`Layer` is where `Figure`s are rendered on. A `Layer` draws a `FigureQueue` at a time. `Layer` itself is also a kind of `Figure` that can be drawn on other `Layer`s.

### DesktopLayer

Each root-level window, or `DesktopFrame` owns a `DesktopLayer` coupled with a `HWND` resource, and is directly managed by `Desktop`. All `Figure`s are finally drawn on `DesktopLayer` and presented on screen.

## Message Handling



With the win32 message loop, all message and `Timer` callbacks are synchornized. The next callback will only be invoked after the current callback returns.

All these virtual function callbacks are eventually driven by messages dispatched from `Desktop`, `DesktopFrame` and other lower-level windows.

### Mouse Messages

Mouse message contains the message type and current mouse position relative to the window.

#### Hit Test

After a window receives a mouse message, it should first determine whether the message is to be handled by itself or to be further processed by any of its child windows. This process is called 'Hit Test`.

Mouse messages are first hit-tested with `HitTest()` callback function and then dispatched to the destination child window after a point transform.

#### Mouse Track

Before receiving a mouse message, the window will firstly be notified with a `NotifyMsg::MouseEnter`,

#### Mouse Capture

After a window acquires the mouse capture, all subsequent mouse messages are directly sent to this window.

### Keyboard Messages

### Notification Messages

Notification messages has no parameter, and currently only includes `MouseEnter`, `MouseHover`, `MouseLeave` and `LoseFocus` that are triggered by mouse or keyboard message dispatchers.

### Timer

`Timer` receives a callback function as its constructor argument, and can be set to invoke the callback periodically.

