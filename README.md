# Sorting Visualizer

Simple sorting visualizer made with C++ and SFML 2.5.1 featuring Lua to extends the application with task and additional sorting algorithms.
That was made manly for educational purpose but i can update it when i feel like it, so don't expect it to be frequent.

And i suck at CMake so... no CMakeList.txt for now.

# Compilation
## Windows

To compile, open solution in Visual Studio (2022 is used for this project) and compile with a x64 comfiguration.

## For other systems

Same as windows but you should compile and/or use your own precompiled version of SFML (2.5.1) and Lua (5.4.6) in x64 and edit linker entry property (not sure if it's so simple but i hope it work).

# Important note after compilation

Don't forget to put DLLs (or other type of dynamic library if you're not a windows user) and "Ressources/" folder in the application folder or else the application will crash immediatly at launch.

### Optional

You can put the "additionalSort/" and "task/" folders in the application folder.

# Alternatively

You can download the Release precompiled binaries instead if you are on windows.

# Run

## Windows

run SortingVisualizer.exe.

## Other systems

run the executable corresponding to your system.

# How to use

## Controls

These controls are for AZERTY keyboard, so that may change if you use QWERTY.

* press right and left arrow to switch algorithm.<br>
* press up and down arrow to increase or decrease array size by factor of 2.<br>
* press enter to launch the algorithm and press escape to stop it.<br>
* press A to switch between sorting algorithms and additional sorting algorithms (implemented in Lua, can be found in the "additionalSort/" folder).<br>
* press Tab to open the task menu where you can choose a task to launch (implemented in Lua, can be found in the "task/" folder).<br>
* press D to switch between step by step mode and normal mode, press enter to make a step in step by step mode.
* press 1 to toggle visualisation of auxiliary array.

## Additional sorting algorithms and task list

* additional sorting algorithms and task list are updated at launch, so if you have created or deleted a lua file, restart the application for the changes to take effect.
* Change in these files don't need application restart.

# Implementing your own additional sorting algorithms and task in lua

* If you want to implementing your own additional sorting algorithms and tasks in lua, you can check respectively the "additionalSort/" and "task/demo/" folders.<br>
* Needed functions for additional sorting algorithms are in the "sv" table and are implemented in C++.<br>
  Here the list :
  * sv.aborted()
  * sv.getSize()
  * sv.setTimeMultiplier(mul)
  * sv.swap(left, right, [access])
  * sv.compare(left, right, [access])
  * sv.write(index, value, [access])
  * sv.copy(dest, src, [access])
  * sv.createAuxiliaryArray(size)

## Additional notes

* sv.getSize return the size of the main array.<br>
* sv.setTimeMultiplier set the time for array access operations (swap, compare, write, copy) before the next operation to happen in visualisation (in seconds).<br>
* The parameters "left", "right", and "index" are indexes and must be unsigned integer in valid range of the array.<br>
* sv.write parameter "value" must be a unsigned integer >= 0 and can't exceed main array size (cause graphical issues if i allow it).<br>
* The parameter "access" is optional and can be one of the following : "mm", "ma", "am", "aa".
  * The first letter is for the first argument and the second letter is for the second argument ('m' to target main array, 'a' to target auxiliary array).
  * sv.write access can only be "mm" or "aa" and take effect only on the first argument.
  * If string empty or invalid, default "mm" will be used.
* When a sorting algorithm as been stopped early (by pressing the escape key), sv.aborted() return true and all array access operations cease to function, so break your loops as soon as you can (usually with return).<br>
* All array access operations return false when arguments are invalid or if sv.aborted() is true, if it's the case, nothing happen. So if one of these functions don't seem to do anything, that may be the cause.<br>
* sv.compare check "left < right" and return the result as the second returned value (the first one is for the function success like mentioned above).<br>
* sv.createAuxiliaryArray can be call once per sorting algorithm (following calls do nothing) and can't be bigger than main array (can change in the future).<br>

I'll probably make a wiki with complete documentation later.<br>

## Warning

* If any parameter is invalid in a "sv" table function, it's likely than the application close, so don't be surprised if that happen.<br>
* I'm not sure this is bug free.<br>
