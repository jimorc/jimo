# ActionHandler2

demonstrates the use of `jimo::theading::ActionHandler` and 
`jimo::threading::ThreadedActionHandler` classes. In this program, an object
derived from `ActionHandler` runs on the program's main thread, and an object derived
from `ThreadedActionHandler` runs on a second thread. This thread is started automatically
in the `ThreadedActionHandler` constructor.

The purpose of this demonstration is to show how action handlers can communicate with each
other.

## Sources

* [ActionHandler2.cpp](ActionHandler2.cpp)
* [CMakeLists.txt](CMakeLists.txt)

## Build and Run

The executable for this program is built as part of the jimo library build process. To 
execute the program, do the following:

Open "Command Prompt" or "Terminal". Navigate to the folder that contains the executable
and type the following:

```bash
./ActionHandler2
```

## Output

The following is sample output from the program.

```
Value has been set to 8
Value has been set to 16
Value has been set to 24
Value has been set to 32
Value has been set to 40
```