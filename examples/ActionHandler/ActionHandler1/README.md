# ActionHandler

demonstrates the use of jimo::theading::ActionHandler class. In this case, there is only
one thread, wth the ActionHandler running in that thread. This is not a `normal` thing to
do.

## Sources

* [ActionHandler1.cpp](ActionHandler1.cpp)
* [CMakeLists.txt](CMakeLists.txt)

## Build and Run

The executable for this program is built as part of the jimo library build process. To excute 
the program, do the following:

Open "Command Prompt" or "Terminal". Navigate to the folder that contains the executable
and type the following:

```bash
./ActionHandler1
```

## Output

The following is sample output from the program.

```
In printIt. Value is 2
In printIt. Value is 3
Start running continuously
In printIt. Value is 6
Waited for 100ms
count = 5
count = 10
count = 14. Will now queue a wait for 250ms
Waited for 250ms
count = 15
count = 20
count = 25
Stopped running continuously
Terminated the handler
Returned from run()
```