# Actions and Action Handling
## Overview
When you look at C-based GUI frameworks, such as Win32, you will notice a switch
statement with an often very long list of case statements that are used to process
GUI messages (events) such as mouse movements, mouse clicks, window opens and closes, and
so forth. 

The processwing of GUI messages is as follows:
* The program creates and displays a window (a frame, dialog, etc.) containing a number of
controls or widgets. When the window is displayed, the program enters a loop that processes
system messages related to the windowing system, such as mouse movements, mouse clicks,
window opens and closes, and so forth.
* The messages are placed in a queue and the messsages are processed one at a time by 
entering a switch statement that contains case labels that match the
system message to process. The code specific to that case is executed, including calling
out to any user code that has registered to be informed of the message or event.
* When the message has been processed, the code goes back to pick up and process the next
message in the message queue.
* When there are no further messages to process, the framework enters an idle or background
loop that processes background or other activities that are not directly driven by system
messages.
* This processing of messages continues until some event, such as closing of the program's
main window, occurs. At that point, the loop is exited and control returns to the program's
main function.

There are are number of cases where similar processing is required in console (non-GUI)
applications. One example would be a program that waits for input from standard input
(std::cin), and processes the words or lines that are input. The words or lines in this
case would be the messages to be processed. The idle or background task would do the waiting for input and queue the messages for processing. As soon as a message has been
queued, the idle or background task would return so that the message queue can be 
processed. Of course, this is not the only way that the program could be designed, but does
illustrate one potential solution.

One requirement of a GUI framework is real-time processing of user input. Any asynchronous
action, such as waiting for input from a hardware device, can lock up the user interface
leading to a bad user experience. The common way for handling this situation is to 
process asynchronous actions in a separate thread. This is known as concurrency.

There are many ways of processing tasks concurrently. If your requirement is the
performance of single tasks, I will leave it to you to determine the best way for your 
situation.
But what is your program needs to connect to a piece of hardware, send messages to,
and receive message from the hardware, and to process continuous input from that hardware
device whose result must be passed back to the main thread? 

One example of this would be a Software Defined Radio (SDR). The application provides a GUI
user interface that interrogates the USB based SDR dongle to determine its current status,
and to set various states based on user input, including continually receiving data from
the device. The data would be processed using digital signal processing and turned into 
audio for output to the computer's audio devices. Certain information, such as signal
strength would be displayed by the application's user interface.

A lot is happening in this case, far too much to perform in a single thread. The
continuous handling of device input places a requirement on the application processing
that likely cannot be handled by concurrency techniques such as asynchronous calls which
start a thread, process a single request, terminate the new thread and return results 
to the calling thread.

The `jimo` library provides a set of two classes and a struct that can be used to satisfy a
number of the requirements that are discussed above: Action, ActionHandler, and
ThreadedActionHandler.

## Action and ActionHandler Library
The following section outlines design requirements for actions and action handlers.

### Design Requirements
A message or action handling library has the following requirements:

* A handler should be able to be used on the main thread.
* A handler should be able to be started in a separate thread.
* A handler must be able to specify and process any number of actions.
* A handler must be able to specify idle or background processing.
* If there are no actions to be performed, and no background processing, then the handler
should wait for a message to process.
* In regards to background processing, there should be actions to start and stop background
processing.
* There must be an action that will terminate the action handler processing loop; otherwise,
it would not be possible to terminate the action handling loop except by throwing an
exception.
* Code that processes an action shall be registered with the action handler. The code that
does the processing should be accepted as a `std::function`. That is, stand-alone functions,
functors, lambdas, and static and instance object methods should be allowed.
* It should be possible to register callbacks or *events* that will be executed as the last
step in the processing of an action. As this may not be required for some actions, this
requirement should be optional; that is, some actions may never call callbacks.
* The callbacks should be accepted as `std::function`s; that is, stand-alone functions,
functors, lambdas, and static and instance object methods should be allowed as callbacks.
* It should be possible to specify two sets of callbacks with the start background
processing action:
  * Callbacks to be executed when processing of the start background action is completed.
  * Callbacks to be called during the background processing. These callbacks could be
  executed every time the background processing is performed, or as determined by the
  application.
* There should be a mechanism allow the addition and removal of callbacks that are to be
called during background processing. Other than background processing, actions are
only processed once and disposed of. If an action is to be repeated, a new action must
be created and queued to the handler. Hence, there is no need to be able to add or remove
callbacks from any other action.
* The action loop shall not use a switch statement with cases. See the subsection below
for the reasons.

#### Switch Statement Code Smell
As noted at the top of this page, when you look at C-based GUI frameworks, 
such as Win32, you will notice a
switch statement with an often very long list of case statements that are used to
process GUI messages. This is considered good practice in procedural languages, but in 
object-oriented languages, this
is considered to be a code smell. Here are a few articles and posts on the subject. While 
the posts discuss this in terms of C# and provide C# examples, the code smell applies
equally to C++.

* [Switch Statement - A Code Smell](
    https://www.c-sharpcorner.com/article/switch-statement-a-code-smell/)
* [Refactoring the Switch Statement Code Smell](
    https://makolyte.com/refactoring-the-switch-statement-code-smell/#:~:text=The%20Switch%20Statement%20code%20smell%20refers%20to%20using,type2%20case%20type3%3A%20return%20data%20specific%20to%20type3)
* and many more...

### Action, ActionHandler and ThreadedActionHandler
In the `jimo` library, one struct and two classes provide action handling processing:

* Action struct - this specifies the message any data to be processed, as well as functions 
(callbacks) to be executed on completion of the processing. These can be viewed
as the equivalent of event handlers or slots in a GUI framework.
* ActionHandler class - this processes actions and performs the equivalent of a GUI
framework's idle or background activity. This class would be used on a console
application's main thread. An object of this class is not useful when the application's
main thread uses a GUI framework. It could potentially be used as the basis of a new
GUI message processing loop, but that is definitely beyond the scope of the `jimo`
library.
* ThreadedActionHandler class - this class is derived from the ActionHandler class. It
starts a thread to handle action processing and continuous input processing. This is
useful in some cases in a GUI application such as the SDR program that I outlined above.

The rest of this document discusses how to use these classes.


