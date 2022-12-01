# OnceTimer

demonstrates the use of jimo::timing::OnceTimer class.

## Sources

* [OnceTimer.cpp](OnceTimer.cpp)
* [CMakeLists.txt](CMakeLists.txt)

## Build and Run

The executable for this program is built as part of the jimo library build process. To execute 
the program, do the following:

Open "Command Prompt" or "Terminal". Navigate to the folder that contains the executable.
For Linux and MacOS, enter the following:

```bash
./OnceTimer
```

For Windows, enter the following:
```cmd
OnceTimer
```

## Output

The following is sample output from the program.

```
OnceTimer::Stop called for a timer that was never started.
tick
tickle
OnceTimer::Fire can only be called once.
```