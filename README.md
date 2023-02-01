# jimo
A library of C++20 general-purpose classes

This library contains a small collection of general purpose classes, including timing and cross-thread communications classes.

## Timing

### StopWatch

The StopWatch class provides funtionality to time operations and to also provide lap timing.

### OnceTimer

The OnceTimer class waits a specified amount of time, or until a specified time, and then fires an
"event". A OnceTimer object can only be used once; that is, it will only fire an event once. 
Any attempt to reuse a OnceTimer object throws an exception.

## Building jimo

While work on this library began on MacOS 13 using C++20, I have moved development to Windows 11 because MSVC has the most complete implementation of
C++20. When development on Windows 11 using MSVC is completed, the library will be ported to Linux using gcc and then MacOS using Apple clang. When
development on Windows 11 is completed, up-to-date building instructions will be provided.

### MacOS

A number of tools are required. Be sure that the following is already installed on your system:

1. Xcode - you can install this from the App Store.

2. Xcode command line tools - open a terminal window and enter:
```zsh
xcode-select - install
```

3. A number of additional tools are required. These are installed using Homebrew. See the [installation instructions](https://docs.brew.sh/Installation) for how to install Homebrew.

4. Once Homebrew is installed, enter the following in the terminal:
```zsh
brew install cmake
brew install doxygen
brew install gcc
brew install ninja
```
gcc is required because much of C++20 is not supported in Apple's version of clang even
though the equivalent version of standard clang does.

5. Download jimo:
```zsh
cd <your-root-projects-directory>
git clone https://github.com/jimorc/jimo.git
```

6. Build jimo:
```zsh
cd jimo
mkdir build
cd build
cmake -D CMAKE_CXX_COMPILER=<path_to_g++_compiler> ..
make
make docs
```
<code>path_to_g++_compiler</code> on Apple silicon systems is <code>/opt/homebrew/bin/g++-XX</code>
where <code>XX</code> is the version of gcc that you installed using homebrew. On Intel
 Macs,
<code>path_to_g++_compiler</code>> is <code>/usr/local/bin/g++-XX</code>.

> **Note**
> As of November 2022, there is no native gdb on Apple silicon. It is possible to debug
using lldb on the command line.
>
> I have been unsuccessful attempting to debug from within VS Code.

As an alternative to building on the command line, you can use an IDE. For that, you will have to follow the IDE specific instructions for using cmake to create project files. You must still build the documentation from a terminal.

### Windows
The following is the minimum required to build the jimo library on Windows 10/11 systems:
1. Install Visual Studio (latest version). Community edition is sufficient. Select:
   * Desktop development with C++
   * In the Installation details section, select C++ Modules for v\<xxx\> build tools as appropriate for whatever architecture you are building
for. If in doubt, install both the x86/64 and arch64 components.
   * Select either *Install while downloading* or *Download all, then install* as you deem appropriate, then click on the button.
  
1. Install Doxygen from the [Doxygen Download Page](https://doxygen.nl/download.html)

1. Open a command prompt and enter:
```cmd
cd <your-root-projects-directory>
git clone https://github.com/jimorc/jimo.git
cd jimo
mkdir build
cd build
cmake ..
cmake --build . --config Debug --target ALL_BUILD -j 6
cmake --build . --config Debug --target docs -j 6
```
   If you wish, replace 6 with the number of cores that your processor has.

### Linux
To be added.
