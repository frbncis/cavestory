# Things to Research

## Build/Compiling

* How to break up the project and use the linker so I don't need to recompile everything everytime?

## Headers

* Why do headers include private methods?

## Class Syntax

What's the difference between doing:

```cpp
ClassName::ClassMethod() {
    // method body here.
}
```

^ this method requires header file.

versus:

```cpp
class ClassName {
public:
    ClassMethod() {
        // method body here.
    }
}
```

^ this method does not require a header file (compiler complains about redeclaration).

## Crashes & Debugging

* How to get stack traces?
* Logging libraries?
