DLStack
=======

DLStack permits the elegant management of downloads on the command line.

Typically, GUI programs download files to a user's downloads directory. This paradigm often leads the downloads directory to becomes a purgatory for files. Users often don't remember if files are important or if documents that need to reside at some other location. When attempting to move files out of their downloads directory, users have to inconveniently sort through entries. 

DLStack allows users to manage the downloads directory as a stack. It provides commands to:
1. List the files in the downloads folder by recency
2. Move the move recently downloaded file(s) to the current (working) directory
3. Pipe the most recently downloaded file to another program (or print to standard output)
4. Discard the most recently downloaded file(s)

These operations encourage a paradigm where the downloads folder is used transiently.

Technical goals:
----------------
- Present a simple and cohesive CLI with sane defaults
  + When providing output to the user:
    + When information is ordered, the most important information is printed to the console last
    + Durations are listed in a human-readable format
    + Important information in the output is colorized
  + If successful, all operations are silent
  + Requires minimal configuration (none in most cases)
- Maintain portability
  + Attempts to maintain compatibility all Unix variants
- Minimize external dependencies
  + Creates a single statically-linked binary

Usage:
-----

`dls` or `dls list` lists all files in the downloads director by modification date (output is colorized):

    [#] dls
    #1  40.mp3                  41 minutes, 48 seconds ago
    #0  some_new_download.img   6 seconds ago

To move last file to the current directory, `pop` it:

    [#] dls pop
    [#] ls
    some_new_download.img
    [#] dls
    #0  40.mp3   44 minutes, 21 seconds ago

An file can be discarded, with `discard`:

    [#] dls 
    #1  40.mp3     45 minutes, 18 seconds ago
    #0  unwanted   5 seconds ago
    [#] dls discard
    [#] dls
    #0  40.mp3   45 minutes, 47 seconds ago 

The `top` command can be used to print the output of the first entry to standard out. It can be used to investigate the content of a file or pipe it to another program. For instance, this can be used to hash files:

    [#] dls
    #0  poem   1 seconds ago
    [#] dls top
    If a packet hits a pocket on a socket on a port,
    And the bus is interrupted as a very last resort,
    ...
    [#] dls top | sha512sum
    ee559fc9fbae42af6a59c3b866ef50f9d44bcb12c44ea4faa8cdccf0d36f96f3  -

The `pop`, `top`, and `discard` commands can be optionally succeeded by a number indicating that the action should be repeated. For instance, `dls pop 2` would move the last two downloads to the current directory. For more, see `dls --help`

Installation:
-------------

DLStack creates a single binary relying only on the C library. Only gcc, make, and git are required to complete the steps below:

Clone the DLStack repository and TOML parsing library:

    [#] git clone --recursive https://www.github.com/flu0r1ne/DLStack

Simply `make` and `make install`. It may be necessary to change the `PREFIX` on your system.

    [#] make
    [#] sudo make install PREFIX=/usr

### Optional:

DLStack will require zero configuration in its default state. Optionally, a TOML configuration file can be created at `~/.config/dls/config`.

    # Change the downloads path, defaults to ~/Downloads
    downloads_path = /home/{user}/Alternative_Downloads

Some programs have a "open in this program" option. These programs can be directed to use the `dls_push` executable to put files in the default downloads directory. This is often cleaner as partial downloads do not appear in the stack.

Limitations
-----------
These are some current limitations that may be remedied in the future.

- DLStack only lists / operates on regular files (e.g. does not work with links or directories)
- Does not recognize partial downloads (e.g. .paritals)
