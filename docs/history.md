# History

The `WndDesign` project has undergone several major refactors over the past few years. Most of the designs and implementations have changed and simplified dramatically, while some others have been carried over to the present day. In this article, I'd like to discuss both and explain how they once functioned and why they were dropped or retained.

## Programming Model

### Programming Languages

I sticked to using C++ since it is the only language I'm relatively familiar with at that time. 

### Dll or Lib

I wrote the previous version intentionally as a dynamically linked library so as to better protect the copyright because it will appear with every program that uses it. But this has caused a lot of problem. As of current version, it is completely a statically linked library.

Making it completely open-source helps me focus on the main logic better, and no other effort will be needed.

### Cross-Platform Support



## Window Tree

### Child Window Management

child window list

### Multi-Parent Support

Although in the current version, a window can only have one parent. It is still possible to display the content of the same window on other windows.

## Window Layout

### Region On Parent

### Non-Client Region

Unlike traditional win32 or web development, each window object is the minimum unit in the *window tree*. There is no such concept like client region, or css box model. The border, padding, margin or scrollbar of a window are usually implemented with Frames or Controls, which are all separate window objects. Nevertheless, they can still produce the same effect through proper combination.

### Style

### Reflow

## Drawing

### Visible Region

### Invalid Region

### Canvas

A window is no longer a canvas for figures to draw on, but it itself is a collection of figures to display.

### Layer Allocation

### Tiling

### Rendering Engine

## Redraw Queue and Reflow Queue
