# Develop Guide

## C++ Programming Style

### C++ Version

This project targets C++ 20.

### Namespaces

Namespaces are declared with `BEGIN_NAMESPACE()` and `END_NAMESPACE()` macros.

Anonymous namespaces take the name `Anonymous`, which is an empty macro.

### Pointers

In most cases, smart pointers like `std::unique_ptr<T>` or `std::shared_ptr<T>` are advised to use.

Raw pointers `T*` should never be explicitly used. Only when necessary, use the wrapped form `ref_ptr<T>` or `alloc_ptr<T>` to indicate the ownership of the pointer.

### Virtual Functions

Define virtual functions with the `virtual` keyword no matter it is in a base class or in a derived class. An overriding virtual function should also contain the `override` specifier. Pure virtual functions end with `pure` macro which will expand to `= 0`.

## Program Startup

The Win32 entrypoint `WinMain()` is defined in `WndDesign\system\win_main.cpp`. It then calls the main function `int main()` that should be defined by user.

Typically in the `main()` function, a main `DesktopFrame` is created and registered via `global.AddWnd()`. Then `global.MessageLoop()` is called to get and process messages.

`Global` is a wrapper for `Desktop`, exposing only a few interfaces for managing `DesktopFrame`. `Desktop` is internally used and should not be directly referenced by user code.

When the last `DesktopFrame` is destroyed, or `global.Terminate()` is called, the message loop breaks and the program ends.

## Window Tree

### Window Definition

Each window type is defined by a class that inherits from `WndObject`, overriding some of its virtual functions to implement custom layout calculating, drawing or message handling logics.

### Child Window Definition and Management

A child window type can be defined in a separate class or in its parent window class, depending on whether the type is used exclusively by its parent window or shared across different types of windows.

A child window object can be managed by its parent window through member object, reference or `child_ptr`, which is an alias for `std::unique_ptr<WndObject>`. The last way is most commonly used, since it requires neither additional dependencies for the parent window to construct the child, nor the need to manually destruct the child window when the parent is destructed, although the other two approaches also have their use scenarios.

The child window registration has nothing to do with keeping the child window resource. Parent windows, especially Layouts, use their internal data structures to manage child windows' resources. In most cases, the parent window maintains a `unique_ptr<WndObject>` to the child window, so that the child window resource can be automatically released when parent window destructs.
