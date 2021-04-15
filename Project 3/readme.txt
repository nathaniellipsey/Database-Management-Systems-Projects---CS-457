CODE DOCUMENTATION
NATHAN LIPSEY
CS 457

THIS FILE IS DOCUMENTATION FOR PROJECT 3.

================================
Compile Instructions (CMAKE):
NOTE: CMAKE is required to generate a MAKEFILE using the CMakeLists.txt
If you are unable to compile the program using CMAKE, send me an email, and I will send you a copy of the program compilable without CMAKE.

1. Decompress the project folder
2. Navigate to FOLDER_NAME/Project3/build
3. Right click, Open Terminal Here
4. Enter the command "cmake .." into the terminal
5. Use the command "make" into the terminal
6. Use the command ./project3 PA3_test.sql to run the project from the test file...

NOTE: If you get an error saying that the current cmake cache is different, delete the build folder, make a new folder of the same name, then try again.

The program has been succesfully re-downloaded and executed with the above instructions. Let me know if you have trouble.

GITHUB LINK: https://github.com/nathaniellipsey/CS-457/

Compiled and run on Linux MINT 20.
===============================

PROJECT DESIGN

This program organizes multiple databases by creating Linux directories using the <filename> library (C++17 or higher required!)
Tables are created using standard <ifstream> and <ostream> commands, with the <filename> library being used to navigate through database subfolders.
Database functions are all written under Database.h, while table functions are all listed under Table.h.

There was less to update compared to last time, but I had some difficulties working with my old code since it was in desperate need of a cleanup. Some efforts had to be taken to clean it up, and even now I think that it could still do with some significant cleanup, which I can hopefully get done before the next project.

Implementing the inner join functionality was very easy. To do it, I had the program scan each column of the checked tables to see if they had matching data types. If they did, the program then scanned to see if any of those data values matched up, printing the columns of the respective tables if they did.

In addition to the inner join function, I reoptimized tables to accept up to 99 columns and rows (it could be more, but this number seemed arbitrarily high enough for any functions for future projects) and I updated the "insert into" and "create" functions to accept up to 99 arguements.
