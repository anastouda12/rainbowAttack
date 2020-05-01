# Rainbow attack

## AUTHOR

43256 Touda lachiri Anas D111

## DESCRIPTION:
This program is an implementation of the rainbow attack.
It allows you to generate a rainbow table then crack some password with.

## GENERATE RAINBOWTABLE:
``` ./rainbowTable generate <size in MB> ```

The computed table will be located in build directory.

## CRACK SOME PASSWORDS:
```./rainbowTable attack <path to file with hashes> ```

Note : considering the structure of file per pair Login _ Hash 

## BUILD:
You need to use the makefile to build up the program.
The makefile is located in root program.

command ``` make ```





