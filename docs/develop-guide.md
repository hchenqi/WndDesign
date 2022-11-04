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

Each window type is defined by a class that inherites from `WndObject`, overriding some of its virtual functions to implement custom layout calculating, drawing or message handling logics.

### Child Window Definition

A child window class can be defined in a separate class or in its parent window class, dependending on whether the child window is used exclusively by its parent window or shared by many different types of windows.

### Child Window Management

A window can manage its child window by object, reference or `child_ptr`.

## Window Layout

### Style Definition

Some windows or figures use styles to .

`ValueTag` allows

## Drawing

## Message Handling
