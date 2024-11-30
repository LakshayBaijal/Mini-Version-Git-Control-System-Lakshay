# AOS ASSIGNMENT 4
# Lakshay Baijal 2024202006

[Screencast from 2024-11-16 01-14-34.webm](https://github.com/user-attachments/assets/237dd371-d331-47dd-9267-9a87e3dfd8ae)

## Features

- **Initialize a Repository:** Set up a new mygit repository.
- **Track Changes:** Add files to the staging area.
- **Commit Changes:** Save snapshots of your project history.
- **View Commit History:** Browse through the commit logs.
- **Object Storage:** Efficiently store and manage objects (blobs and trees).
- **Custom SHA-1 Implementation:** Generate unique identifiers for objects.

## Files 
### include/: Contains all header files declaring the classes and their methods.

- add.h: Declaration for the Add class handling the add command.
- cat_file.h: Declaration for the CatFile class handling the cat-file command.
- commit.h: Declaration for the Commit class handling the commit command.
- hash_object.h: Declaration for the HashObject class handling the hash-object command.
- init.h: Declaration for the InitRepository class handling the init command.
- ls_tree.h: Declaration for the LsTree class handling the ls-tree command.
- log.h: Declaration for the Log class handling the log command.
- sha1.h: Declaration for the SHA1 class implementing SHA-1 hashing.
- write_tree.h: Declaration for the WriteTree class handling tree object creation.

### src/: Contains all source files implementing the functionality.

- add.cpp: Implementation of the Add class.
- cat_file.cpp: Implementation of the CatFile class.
- commit.cpp: Implementation of the Commit class.
- hash_object.cpp: Implementation of the HashObject class.
- init.cpp: Implementation of the InitRepository class.
- ls_tree.cpp: Implementation of the LsTree class.
- log.cpp: Implementation of the Log class.
- main.cpp: Entry point of the application handling command-line arguments.
- sha1.cpp: Implementation of the SHA1 class.
- write_tree.cpp: Implementation of the WriteTree class.
- makefile: Build script to compile the project.

- README.md: Project documentation.

# Commands
## Initialize Repository - a new mygit repository in the current directory by creating a .mygit directory containing necessary subdirectories and files.

./mygit init

Creates .mygit/objects, .mygit/hooks, .mygit/info directories.

Creates empty .mygit/HEAD, .mygit/config, and .mygit/description files.

## Hash Object - Computes the SHA-1 hash of the specified file. Optionally writes the file content to the object store.

./mygit hash-object [-w] <file>

## Cat File - Displays information about a Git object.

./mygit cat-file <flag> <object_sha>

## Add Files - Adds specified files or all files in the current directory to the staging area (index).

./mygit add [file1 file2 ... | .]


## Commit Changes - Creates a new commit with the staged changes and an optional commit message.

./mygit commit -m "Commit message"


## View Commit Log - Displays the commit history in reverse chronological order (latest to oldest).

./mygit log


## Write Tree - Writes the current state of the index to a tree object and outputs its SHA-1 hash.

./mygit write-tree


## List Tree - Lists the contents of a tree object.

./mygit ls-tree [--name-only] <tree_sha>

In my git compression and decompression isnt implemented.




