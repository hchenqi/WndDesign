# History

The `WndDesign` project has undergone several major refactorings over the past few years. Most of the designs and implementations have changed and simplified dramatically, while some others have been carried over to the present day. In this article, I'd like to discuss both and explain how they once functioned and why they were dropped or retained.

## Window Tree

## Window Layout

### Client and Non-Client Region

Unlike traditional win32 or web development, each window object is the minimum unit in the *window tree*. There is no such concept like client region, or css box model. The border, padding, margin or scrollbar of a window are usually implemented with Frames or Controls, which are all seperate window objects. Nevertheless, they can still produce the same effect through proper combination.

### Style

### Reflow

## Drawing

### Visible Region

### Tiled Layer and Tiling Cache

## Redraw Queue and Reflow Queue

## Dll or Lib

I wrote the previous verion intentionally as a dynamically linked library so as to better protect the copyright. But this has caused a lot of problem. As of current version, it is completely a statically linked library.
