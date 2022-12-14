# jimo
A library of C++20 general-purpose classes

This library contains a small collection of general purpose classes, including timing and cross-thread communications classes.

## Timing

### StopWatch

The StopWatch class provides funtionality to time operations and to also provide lap timing.

## Building jimo

This library is being developed on MacOS 13 using C++20 and have just started porting it to Windows 11.

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
```

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
cmake ..
make
make docs
```
As an alternative to building on the command line, you can use an IDE. For that, you will have to follow the IDE specific instructions for using cmake to create project files. You must still build the documentation from a terminal.

### Windows
The following 
1. Install Visual Studio (latest version). Community edition is sufficient. Select:
   * Desktop development with C++
   * In the Installation details section, select C++ Modules for v<xxx> build tools as appropriate for whatever architecture you are building
for. If in doubt, install both the x86/64 and arch64 components.
   * Select either <i>Install while downloading</i> or <i>Download all, then install</i> as you deem appropriate, then click on the button.
  
2. Install CMake from the [CMake Download Page](https://cmake.org/download/).

3. Install Doxygen from the [Doxygen Download Page](https://doxygen.nl/download.html)

3. Open a command prompt and enter:
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
