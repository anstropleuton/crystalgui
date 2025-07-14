# 
[![Uses C++23](https://img.shields.io/badge/C++-23-blue.svg)](https://en.cppreference.com/w/cpp)
[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](license.md)
[![Latest Version v](https://img.shields.io/badge/Latest-v-yellow.svg)](https://github.com/anstropleuton/crystalgui/releases/latest)

# Prerequisite

- A C++ compiler that supports [C++23](https://en.cppreference.com/w/cpp/compiler_support/23)
- CMake 3.15 or higher
- [doctest](https://github.com/doctest/doctest) (for testing)

# Building

- Clone the repository
```bash
git clone https://github.com/anstropleuton/crystalgui.git
```
- Create a build directory
```bash
mkdir build
cd build
```
- Build the project
```bash
cmake ..
cmake --build . --config Release
```
- Install the project (optional; UNIX-like systems)
```bash
sudo cmake --install . --config Release
```

# Quick-Start Example

If you are ready to dive into the APIs, add your project as a subdirectory in your CMakeLists.txt:
```cmake
add_subdirectory(crystalgui)
```

If your project does not use CMake for your project, you can use CMake to build Fluxins as a static library, set up include paths, and link the library to your project.

```cpp


#include "crystalgui/crystalgui.hpp"

int main()
{
}
```

# Features

# Anti-features

# Future plans/considerations/TODOs

# General Information

# More Examples

There are a few examples that I have created to demonstrate Crystalgui.

# Contributing

Feel free to contribute to this project by code (see [contributing.md](contributing.md)), by suggesting your ideas, or even by reporting a bug.

# Credits

Thanks to [jothepro](https://github.com/jothepro) for the stylesheet [Doxygen Awesome](https://github.com/jothepro/doxygen-awesome-css) for Doxygen.

Thanks to Nuno Pinheiro (can't find link) for the background Elarun. But I did find a [KDE store link](https://store.kde.org/p/1162360/) to the background image. I think you could consider that. If KDE developers are here, I would appreciate your help to find a proper link to the author.

Thanks to [patorjk](https://www.patorjk.com) for [Text to ASCII Art Generator](https://www.patorjk.com/software/taag).

# License

Copyright (c) 2025 Anstro Pleuton.

This project is licensed under the terms of MIT License. See [license.md](license.md) for more info.
