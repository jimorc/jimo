# Event

demonstrates the use of jimo::Event class.

## Sources

* [Event.cpp](Event.cpp)
* [CMakeLists.txt](CMakeLists.txt)

## Build and Run

The executable for this program is built as part of the jimo library build process. To excute 
the program, do the following:

Open "Command Prompt" or "Terminal". Navigate to the folder that contains the executable
and type the following:

```bash
./Event
```

## Output

The following is sample output from the program.

```
From running event2
All 4 values have been set:
ObjectWithEvents: 1, 8, 4, 16
From running event2
Only the first and third values are set after two std::functions are removed from event1
ObjectWithEvents: 4, 8, 7, 16
```