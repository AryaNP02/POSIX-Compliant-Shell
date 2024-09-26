
# POSIX Shell Implementation

This project is an implementation of a POSIX-compliant shell .

## How to Run

To execute the shell, follow these steps:

1. **Build the Project**

   First, ensure you have CMake installed. Navigate to the project's directory and use the following commands to build the project:

   ```bash
      make

2. **Run**

   You can now run the shell as: 
```
     ./shell.out 
```
4. **Alternative approach**

   ```bash
     g++ main.cpp setup.cpp doubleLinkList.cpp cd.cpp echo.cpp pwd.cpp ls.cpp pinfo.cpp search.cpp history.cpp redirection.cpp 
     prompt.cpp -o shell.out

   Then follow step 2 .

## Project Structure

- **`main.cpp`**: Initializes signal handling, command history, autocomplete functionality, and input parsing.

- **`setup.cpp`**: Sets up the shell environment and configurations.

- **`doubleLinkList.cpp`**: Implements a doubly linked list used for efficiently managing of command history.

- **`cd.cpp`**: Implements the `cd` command to change directories  (Linux `cd` command).

- **`echo.cpp`**: Implements the `echo` command to print text (Linux `echo` command).

- **`pwd.cpp`**: Implements the `pwd` command to display the current directory (Linux `pwd` command).

- **`ls.cpp`**: Implements the `ls` command to list files and directories (Linux `ls` command).

- **`pinfo.cpp`**: Implements the `pinfo` command to show process information (Linux `pinfo` command).

- **`search.cpp`**: Implements a recursive search for files and directories (Linux-like search functionality).

- **`history.cpp`**: Manages command history, including saving and retrieving previous commands.

- **`redirection.cpp`**: Handles I/O redirection (input, output, append), piping, and handling calling of commands.

- **`prompt.cpp`**: Controls the display of the shell prompt and manages user input.

### Header Files

- **`shell.h`**: The master header file where all function prototypes and global variables are defined.

- **`doubleLinkList.h`**: Contains function prototypes for managing the doubly linked list used in the project.

## Functionality

1. **Display Requirement**: Dynamically show `_username_@_system_name_:_current_directory_>` on the prompt. Handle spaces and tabs.

2. **Basic Commands**: Implement `cd`, `echo`, and `pwd`. Ensure `cd` updates the current directory, `echo` prints strings correctly, and `pwd` shows the current working directory.

3. **ls Command**: Implement `ls` with `-a` and `-l` flags, handling various cases and arguments.

4. **System Commands**: Execute foreground and background processes, managing multiple background tasks and printing their PIDs.

5. **pinfo Command**: Display process information for the shell or a given PID without `popen()`.

6. **search Command**: Recursively search for files or folders in the current directory, outputting `True` or `False`.

7. **I/O Redirection**: Support input/output redirection with `<`, `>`, and `>>`, handling file creation, overwriting, and appending.

8. **Pipeline**: Handle command pipelines using `|`, supporting multiple pipes.

9. **Redirection with Pipeline**: Manage input/output redirection within command pipelines.

10. **Simple Signals**: Implement handling for `CTRL-Z` (suspend), `CTRL-C` (interrupt), and `CTRL-D` (logout).

11. **Autocomplete**: Implement TAB key autocomplete for files and directories.

12. **History**: Manage up to 20 commands with the `history` command, display up to 10 commands by default, support `history <num>`, and handle UP arrow key navigation.


## Additional Features

- If the received command matches any of the listed commands (above), it will be handled according to the implementation, even if it is part of a pipeline or redirection. However, if the command involves piping or redirection and doesn't meet the specified structure mentioned above, unexpected behavior may occur.

- Anything inside Quotes are handled as strings in the implementation, and additionaly  escape sequences are handeled.

- The terminal can execute other shell script.
