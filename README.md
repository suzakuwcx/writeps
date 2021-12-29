# writeps
Send a message to a process stdin

Some code is refering to the writevt.c contained in console-tools , but I can't find the author.
the sources of that file is the link below :

https://github.com/jr-k/writevt

## Usage

```shell
#  send a message to the process by pid
    writeps <pid> <message>
``` 

## Example

in the first console, use a command to waiting input in stdin
```shell
[root@host /]& cat 
```

open another console, send a message to the origin

```shell
# get process pid
[root@host /]& pidof cat
23914
[root@host /]& writeps 23914 "a message"
```
you can see the response in the first console

## Compile

```shell
gcc -o writeps writeps.c
```

## NOTE

Please note that, some programs don't receive user input from their stdin.
