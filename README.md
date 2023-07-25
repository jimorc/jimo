# jimo
A library of C++20 general-purpose classes

This library contains a small collection of general purpose classes, including timing and 
cross-thread communications classes.

## Timing

### StopWatch

The StopWatch class provides funtionality to time operations and to also provide lap timing.

### OnceTimer

The OnceTimer class waits a specified amount of time, or until a specified time, and then fires an
"event". A OnceTimer object can only be used once; that is, it will only fire an event once. 
Any attempt to reuse a OnceTimer object throws an exception.

## Building jimo

### Current State of Development

While work on this library began on MacOS 13 using C++20, I have moved development to Windows 11 
because MSVC has the most complete implementation of
C++20. When development on Windows 11 using MSVC is completed, the library will be ported to Linux 
using gcc and then MacOS using Apple clang. When
development on Windows 11 is completed, up-to-date building instructions will be provided.

### Command Line

It is possible to build the jimo library, tests, example applications, and its documentation just 
using command line tools. Instructions are provided below for building jimo using the command line 
on each of MacOS, Windows, and Linux.

If you want to use the jimo library and will not be modifying it, then it is not even necessary to 
perform a build because the library is header based, and no executable is needed, or even produced 
during the build process. All documentation is provided online. You will, however, need to follow
the build instructions if you want to install the library on your system rather than just use it 
within a single C++ project. More instructions on installing the library will be provided when 
development is complete.

As an alternative to building on the command line, you can use an IDE. For that, you will have to 
follow the IDE specific instructions for using cmake to create project files.

### Using an IDE

The source for this library includes CMake files for building the library. You can  use any IDE that
allows the importation of CMake files. This includes pretty much every IDE that supports C++ today.

I use VSCode as an editor/IDE because it runs on all of the platforms that I will build the jimo 
library on. Instructions for installing VSCode on each of the platforms are included in the 
appropriate sections below. However, you can use any IDE or set of tools that you want if you are
doing any development. And you do not need any of the tools if you simply intend to use the jimo 
library and not do intend to make any changes to the jimo library.

 #### My Setup for VSCode

 Here are the tools, programming languages, and VSCode extensions that I use to build the library 
 with VSCode.

 1. VSCode. See the [VSCode download page](https://code.visualstudio.com/download) for executables. 
 Microsoft provides extensive documentation about how to set up VSCode for C++ development.

 2. The latest Python. Python is required to provide some functionality in VSCode. See the
  [Python download page](https://www.python.org/downloads/) for further details. 
  Note that on Linux and some other systems, Python is also available through package installers.

 3. The following VSCode extensions:
     - C/C++ Extension Pack
     - C++ TestMate
     - GoogleTest Adapter
     - Doxygen Documentation Generator

4. I also find the following extensions useful, but they are not essential:
     - Git History
     - Print

### Building on MacOS
*Note: This section is out of date. Attempts to build the jimo library using the here will result in 
a large number of errors. I am simply maintaining it here to prevent having to recreate all of this 
information when I have completed development on MacOS.

A number of tools are required. Be sure that the following is already installed on your system:

1. Xcode - you can install this from the App Store.

2. Xcode command line tools - open a terminal window and enter:
```zsh
xcode-select - install
```

3. A number of additional tools are required. These are installed using Homebrew. See the 
[installation instructions](https://docs.brew.sh/Installation) for how to install Homebrew.

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

### Building on Windows
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

### Building on Linux
To be added.
