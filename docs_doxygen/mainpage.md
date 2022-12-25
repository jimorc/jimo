# What is the jimo Library? Introduction and Overview

The jimo Library contains a small set of utility classes. They are separated into three subsets:
1. General classes:
    * Delegates, Events, and Event Handlers
2. Timing Classes
    * StopWatch - for timing activities. It includes lap timing.
    * Timer - a class that acts as a clock. It can fire an event after a specified time or at specified intervals.
    * StopWatch and Timer Exception classes. 
3. Interthread Communications Classes
    * Classes to be created.

## Features
* Free and open source ([MIT License](https://github.com/jimorc/jimo/blob/main/LICENSE)).
* A collection of native C++ classes to implement functionality that is not included in std.
* Full integration with std.
* Written in efficient, modern C++20 with the 
[RAII](https://github.com/jimorc/jimo/blob/main/LICENSE) programming idiom.
* Highly portable and available on Windows, MacOS and Linux. The should be portable to
iOS and Android as well.

## jimo Library Architecture
The jimo Library consists mainly of templated classes, but also a few non-templated classes that are contained within a library file. jimo is divided into three parts:
### jimo.core
jimo.core contains the general classes related to delegates, events, and event handlers. These classes can be used directly within your code, and are also be used within the other parts of the jimo Library.
### jimo.timing
jimo.timing contains the StopWatch, Timer, and related exception classes.
### jimo.interthread
jimo.interthread contains classes for interthread communication.

## Getting Started
To be written.