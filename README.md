# jimo
A library of C++20 general-purpose classes

This library contains a small collection of general purpose classes, including timing and cross-thread communications classes.

## Timing

### StopWatch

The StopWatch class provides funtionality to time operations and to also provide lap timing.

## Building jimo

This library is being developed on MacOS 13 using C++20. After some functionality is added to this library, it will be ported to Windows and Linux.

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
To be added

### Linux
To be added.