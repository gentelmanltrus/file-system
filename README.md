# file system manager with support for virtual file systems

## Project Description

This project develops a file system manager that supports virtual file systems.
Virtual file systems are completely separate from the operating system's file system, 
with unique file hierarchies that store file objects mapped to physical paths in the 
underlying operating system.
File system manager supports standard file system manipulation commands for both 
physical and virtual file systems. Works on Windows and Linux. File system paths with spaces are not supported.

## Commands

### Shared Commands 

#### Help (help)

```
help
```

Lists all available commands with their descriptions.

#### Print Working Directory (pwd)

```
pwd
```

Prints the current working directory to the terminal. Also works within virtual file systems, printing the virtual path instead.

#### List (ls)

```
ls <path>
ls
```

Lists all items in the directory at the given path. Without arguments, lists the contents of the current working directory. Also works within virtual file systems, listing all objects in the virtual directory at the given virtual path or the contents of the current virtual directory instead.

#### Change Directory (cd)

```
cd <path>
```

Changes the current working directory using absolute or relative file paths. Also works within virtual file systems, using virtual file paths.

#### Touch (touch)

```
touch <path/file-name>
```

Creates an empty file at the specified absolute or relative path. Throws an error if a file with the same name already exists at that path. Also works within virtual file systems — creates a virtual file object mapped to the given path, as well as a real file inside virtual_folder in the program directory.

#### Make Directory (mkdir)

```
mkdir <path/directory-name>
```

Creates a directory at the specified absolute or relative path. Does nothing if a directory with the same name already exists at that path. Also works within virtual file systems — creates a virtual directory object that can store virtual file objects mapped to their respective physical paths.

#### Remove (rm)

```
rm [opptional flag: -f, -s, or both] <path/item-name>
```

Removes a file system item at the specified absolute or relative path, with a confirmation prompt. Also works within virtual file systems, deleting only the virtual file system object by default. Includes optional flags:

##### Force (-f)

Immediately deletes an item without a confirmation prompt.

##### Strict (-s)

Deletes the virtual file system object along with its corresponding physical file inside the virtual folder in the program directory. Throws an error if the current file system is physical.

#### Alias (alias)

```
alias <command-name> <alias-name>
alias
```

Creates an alias for a given command. Creates an alias for a given command. Running `alias` without arguments lists all existing aliases and their corresponding commands.

#### Unalias (unalias)

```
unalias <alias-name>
```

Removes the alias with the provided name.

#### Quit (quit)

```
quit
```

Exits the program.

### Physical File System Commands

#### Tree (tree)

```
tree
```

Prints the full file system hierarchy tree starting from the current working directory. Only works within physical file system.

#### Report (report)

```
report
```

Reports the count of each file type and their respective largest file with full path, starting from the current working directory. Only works within physical file systems.

#### Duplicates (duplicates)

```
duplicates
```

Reports all duplicate files found starting from the current working directory, including their count, size, and full path. Only works within physical file systems.

### Virtual File System Commands

#### Create (create)

```
create <file-system-name> [import-full-path]
```

Creates a new empty virtual file system. If an optional path is provided, imports an existing file system item into the virtual file system as a new file system object.

#### Import (import)

```
import <absolute-path/item-name>
```

Creates a virtual file object from an absolute path to a physical file. Only works within virtual file systems.

#### Switch (switch)

```
switch <file-system-name>
```

Switches the current file system to the one with the provided name.

#### Delete (delete)

```
delete <file-system-name>
```

Deletes the specified virtual file system. Throws an error if a physical file system name is provided.

## Launching the Application

From the project directory, run `mingw32-make` on Windows or `make` on Linux. Then run `./file-system.exe`.

On Windows:
```cmd
mingw32-make
.\file-system.exe
```

On Linux:
```bash
make
./file-system.exe
```

