# Event1

demonstrates the use of jimo::Event class.

## Sources

* [Event1.cpp](Event1.cpp)
* [CMakeLists.txt](CMakeLists.txt)

## Build and Run

The executable for this program is built as part of the jimo library build process. To excute 
the program, do the following:

Open "Command Prompt" or "Terminal". Navigate to the folder that contains the executable
and type the following:

```bash
./Event1
```

## Output

The following is sample output from the program.

```
Before events are invoked:
ObjectWithEvents: 0, 0, 0, 0
From running event2
Only the second and third values are set because event processing is halted after the second value is set:
ObjectWithEvents: 0, 8, 4, 0
From running event2
The second value will not be changed after ftor is removed from event1
ObjectWithEvents: 4, 8, 7, 19
```