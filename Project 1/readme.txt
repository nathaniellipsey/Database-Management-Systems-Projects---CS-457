CODE DOCUMENTATION
NATHAN LIPSEY
CS 457

THIS FILE IS DOCUMENTATION FOR PROJECT 1.

================================
Compile Instructions (CMAKE):
NOTE: CMAKE is required to generate a MAKEFILE using the CMakeLists.txt
If you are unable to compile the program using CMAKE, send me an email, and I will send you a copy of the program compilable without CMAKE.

1. Decompress the project folder
2. Navigate to FOLDER_NAME/Project1/build
3. Right click, Open Terminal Here
4. Enter the command "cmake .." into the terminal
5. Use the command "make" into the terminal
6. Use the command ./project1 to run the project

NOTE: If you get an error saying that the current cmake cache is different, delete the build folder, make a new folder of the same name, then try again.

The program has been succesfully re-downloaded and executed with the above instructions. Let me know if you have trouble.

GITHUB LINK: https://github.com/nathaniellipsey/CS-457/

Compiled and run on Linux MINT 20.
===============================

PROJECT DESIGN

This program organizes multiple databases by creating Linux directories using the <filename> library (C++17 or higher required!)
Tables are created using standard <ifstream> and <ostream> commands, with the <filename> library being used to navigate through database subfolders.
Database functions are all written under Database.h, while table functions are all listed under Table.h.

I designed this project to work based off the test.sql file provided with project 1. 
This current version does not support input from file. 
Instead, the commands can be typed into the terminal, where they are read in-- the program checks to see if it recognizes any of the commands, and then checks the syntax. 
If the command is recognized and the syntax is correct, it makes sure that the input is valid before running the appropriate function.

Functions for the database and tables are stored in two seperate documents. 
Because they are seperated, the main.cpp program makes sure that a database is selected before the user can modify or create any tables. 
The create database function utilizes the <filesystem> library from c++ 17 to create a directory with the name of the database. 
The drop database function also uses the <filesystem> library to likewise remove a database. 
A dataBaseExists function returns true if it can find a folder with the given name (again, using <filesystem>). 
The table functions are nearly identical, with one exception-- they create .dat files instead of folders, and they modify those .day files using the <fstream> library. 
Finally, the main function loops indefinately with a while (1) loop, returning 0 when it recieves the .EXIT command. 
It takes in up to 9 arguements (reading until it reaches its max or until it reads a semicolon), then compares the input with the known functions.
