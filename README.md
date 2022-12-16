# "The Great Firewall of Santa Cruz" [CSE 13S]
This program filters out "badwords" using regular expressions and prints out 
the violations made. If there is a newspeak value associated with 
the oldspeak word, then that is also printed. 

## Building
Build the program with:
```
$ make
```
You can check the formatting using:
```
$ make format
```
Remember to clean up afterwards so there are no object files or executables left over:
```
$ make clean
```

## Running
Run the program using:
```
$ ./banhammer
```
followed by a command where:
```
-h help
-s print program statistics
-t size of hash table (2^16 by default)
-f size of bloom filter (2^20 by default)
```
For example, you can run ./banhammer -s with some stdin text to print out 
the average BST size, height, banches, hash table and bloom filter load.
If statistics are printed (-s), then the badspeak words that are in violation of the 
rules and the message are not printed.

## Possible Errors
No bugs, errors or memory leaks detected by scan-build and valgrind.
