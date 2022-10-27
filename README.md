# WndDesign

A C++ GUI library

## Quick Start

*for Windows 10/11 systems only*

* Install Visual Studio 2022, check **Desktop development with C++**.

* Clone `WndDesign` source code.

* Click open `WndDesign.sln`, set `Test` as startup project, build, and run.

## Concepts

### Window Tree

#### WndObject

A window, or `WndObject`, is the basic unit that draws figures and handles messages.

Each `WndObject` may have none, one, or multiple child windows, and each `WndObject` may have only one parent window.

All `WndObject` with child windows and parent windows form a *window tree*.

#### Control, Frame and Layout

Controls, Frames and Layouts are all `WndObject` but they play different roles in the *window tree*.

![](docs/component.png)

##### Control

Controls have no child window, so they are always leaf nodes in the *window tree*. Controls include `Button`, `TextBox`, `EditBox`, `ImageBox`, `Scrollbar` and `Placeholder`. They are often used to handle user inputs or display simple figures.

##### Frame

Frames have one child window. Frames include `BorderFrame`, `PaddingFrame`, `ScaleFrame`, `ClipFrame`, `ScrollFrame` and `LayerFrame`. They are often used to decorate a window with border or padding, or change the window's resizing or drawing behaviour.

##### Layout

Layouts may have multiple child windows. Layouts include `SplitLayout`, `ListLayout`, `FlowLayout`, `BarLayout` and `OverlapLayout`. They are often used to display complex data structures.

#### Desktop

`Desktop` is the root window in the *window tree*.

![](docs/Desktop.png)

`DesktopFrame` is the direct child window of `Desktop`. Each `DesktopFrame` displays an overlapped window on desktop.

### Window Layout

The layout of a `WndObject` is how its contents or child windows are organized and how their relative positions and sizes are calculated.

#### 1D Geometry

##### Position

`position` is a signed value relative to the origin point.

##### Length

`length` is an unsigned value indicating the length of a line segment.

#### 2D Geometry

##### Point

`Point` is a pair of position values `x` and `y`, which is always relative to a specific window. The upper-left corner of a window is usually taken as the origin point (0, 0).

##### Size

`Size` is a pair of length values `width` and `height`. 

##### Rect

`Rect` is a pair of `Point` and `Size` that represents a rectangular region. It is also relative to a specific window.

##### Vector

`Vector` is the difference between two `Point`s. It can be applied to a `Point` or a `Rect` as a translation transform.

#### Layout Type

Layout type indicates how a window's width or height will change according to its parent window's width or height.

There are three kinds of layout types for both horizontal and virtical dimensions, `Assigned`, `Auto` and `Relative`.

##### Assigned

Indicates the width or height of a window is assigned by its parent window.

##### Auto

Indicates the width or height of a window is independent of its parent window, and is probably dependent on its contents or child windows.

##### Relative

Indicates the width or height of a window is dependent on its parent window but is not strictly assigned by its parent window. 

### Figure

Figures are what a window can display on the screen. `Figure` includes `Line`, `Rectangle`, `RoundedRectangle`, `Ellipse`, `Image` and `TextBlock`.

### Message

Messages are what a window may respond to for handling user input or timer alerts. Messages include mouse messages like `LeftDown` and `RightUp`, keyboard messages like `KeyDown` and `KeyUp`, and notification messages like `MouseLeave` and `LoseFocus`.
