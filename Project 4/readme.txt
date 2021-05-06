CODE DOCUMENTATION
NATHAN LIPSEY
CS 457

THIS FILE IS DOCUMENTATION FOR PROJECT 4.

================================
Compile Instructions (CMAKE):
NOTE: CMAKE is required to generate a MAKEFILE using the CMakeLists.txt
If you are unable to compile the program using CMAKE, send me an email, and I will send you a copy of the program compilable without CMAKE.

1. Decompress the project folder
2. Navigate to FOLDER_NAME/Project4/build
3. Right click, Open Terminal Here
4. Enter the command "cmake .." into the terminal
5. Use the command "make" into the terminal
6. Use the command ./project4 PA1.sql to run the project from the test file...

NOTE: If you get an error saying that the current cmake cache is different, delete the build folder, make a new folder of the same name, then try again.

The program has been succesfully re-downloaded and executed with the above instructions. Let me know if you have trouble.

GITHUB LINK: https://github.com/nathaniellipsey/CS-457/

Compiled and run on Linux MINT 20.
===============================

PROJECT DESIGN

My goal with this program was to not only write a program that fulfilled the requirements listed, but also to clean up a large amount of the messy code from my previous projects to make it easier to work with and modify. This alone took a great deal of time-- about 4 hours-- but ultimately the code was cleaner, easier to work with, and less bug prone than before.

Furthermore, I implemented a few features that I had neglected in Project 3. Most notably, the program can now read a table in from an existing file, modify it, and then rewrite the old table. Furthermore, I created 3 new definitions (MAX_ROWS, MAX_COLUMNS, MAX_TOKENS) to set the bounds for the tables rows and columns and the amount of inputs that will be accepted respectively. By default, these are all set to 99 (meaning that the data table can hold 99 rows and 99 columns) but these can be raised to the highest possible number an integer can accept.

Some other, more minor things that I changed from my last project-- external functions are now forward declared. The program can now switch between multiple tables if given files. Tables columns are now a class instead of a struct, and include some unique functions. The table itself is now a class instead of an array of structs. The table class object has several functions that allow it to create table locks, push new values, print itself to the terminal or to a file, change its own name, load a table from a file, update the table, and clear the table. The inputs are now referred to within the code as "tokens". The main() function no longer handles inputs, but rather passes them along to a seperate function, "executeTokens(string tokens[])". The database creation and insert functions both check for proper syntax.

Finally, I added several new functions to meet the project requirements: commit and begin. The update function was also changed to account for this.

All in all, despite optimization, the program wasn't much shorter, sitting at 750 lines (discluding headers). The program also ran into an odd misfunction-- one that would cause the printToConsole() function to not print the proper number of newlines (an error which, as of the current version, is now fixed-- the table read function was causing it to read too many lines in). Overall I am very proud of the project as a whole and I think I did a good job on it.
