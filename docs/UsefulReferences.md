# Useful References

Links to references that were useful as I trawl through learning and building with C++.

## Style Guides

* <https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines>

## SDL2

* <https://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/cli/index.php>

## Project Structure

* <https://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/>
* <https://dev.to/dmendoza99/how-to-write-a-good-makefile-for-c-32e3>

## Debugging with `gdb`

* In Visual Studio Code, you can execute `gdb` commands in the debug console using `-exec <command>`
* Viewing memory contents:

  * [Reference](https://sourceware.org/gdb/onlinedocs/gdb/Memory.html)
  * Viewing four integers worth of contents: `-exec x/4wd &source_rect`
  * "Hexdump"?: `-exec x/16wb &source_rect`
