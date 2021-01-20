DLStack
=======

DLStack permits the management of downloads via terminal. 

Typically, GUI programs download files to a user's downloads directory. This paradigm often leads the downloads directory to becomes a purgatory for files. User often don't remember if files are important or if documents that need to reside in some other location. When attempting to move files out of this directory, users have to inconveniently sort through entries. DLStack is a simple C executable with no external dependencies that allows users to manage the downloads directory as a stack. It makes this terminal management simple and convenient.

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

An file can be discarded, with discard:

    [#] dls 
    #1  40.mp3     45 minutes, 18 seconds ago
    #0  unwanted   5 seconds ago
    [#] dls discard
    [#] dls
    #0  40.mp3   45 minutes, 47 seconds ago 

The `top` command can be used to print the output of the first entry to standard out:

    [#] dls
    #0  poem   1 seconds ago
    [#] dls top
    If a packet hits a pocket on a socket on a port,
    And the bus is interrupted as a very last resort,
    ...
    [#] dls top | sha512sum
    ee559fc9fbae42af6a59c3b866ef50f9d44bcb12c44ea4faa8cdccf0d36f96f3  -

For more, see `dls --help`

Installation:
------------

git clone --recursive https://www.github.com/flu0r1ne/DLStack

Simply `make` and `make install`. It may be necessary to change the `PREFIX` on your system.

    [#] make
    [#] sudo make install PREFIX=/usr

Optionally, a TOML configuration file can be created at `~/.config/dls/config`.

    # Change the downloads path, defaults to ~/Downloads
    downloads_path = /home/{user}/Alternative_Downloads

Optionally, some programs have a "open in this program" option. These programs can be directed to use the `dls_push` executable to put files in the default downloads directory. This is often cleaner as partial downloads do not appear in the stack.

Limitations
-----------
These are some current limitations that may be remedied in the future.

- DLStack only lists / operates on regular files
- Does not recognize partial downloads (e.g. .paritals)
