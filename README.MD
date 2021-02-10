# Rainbow attack

- Built with C++

## AUTHOR

43256 Touda lachiri Anas D111

## DESCRIPTION:
This program is an implementation of the rainbow attack.
It allows you to generate a rainbow table then crack some password of length 8.
The program used 4000 reduction function between the head and the tail.
Increasing the length of the chain decreases the size of the table but it also increases the time required to perform lookups, and this is the time-memory trade-off of the rainbow table.
So to meet the requierements of 45 minutes +- lookups of 100 passwords, 4000 reduction function was the optimal.
So pleaze considear the size of the rainbowtable when generating.

history of generation : Generation of 40MB rainbowTable on 8 hours.
history of crack : Crack on 40 minute.

## BUILD:
You need to use the makefile to build up the program.
The makefile is located in root program.

command ``` make ```

## USAGE:

Use that followings command in the directory /build

``` 
   ./RainbowTable command option, where :
   	- command 'generate' to generate a rainbow table
    - command 'attack' to execute a rainbow attack
    - option size of table in MB with command 'generate'
    - option path to file containing hashes password with command 'attack'
 ``` 

## GENERATE RAINBOWTABLE:
``` ./RainbowTable generate <size in MB> ```

The computed table will be located in build directory.

## CRACK SOME PASSWORDS:
```./RainbowTable attack <path to file with hashes> ```






