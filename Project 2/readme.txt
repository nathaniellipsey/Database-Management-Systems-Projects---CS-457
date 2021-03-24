CODE DOCUMENTATION
NATHAN LIPSEY
CS 457

THIS FILE IS DOCUMENTATION FOR PROJECT 2.

================================
Compile Instructions (CMAKE):
NOTE: CMAKE is required to generate a MAKEFILE using the CMakeLists.txt
If you are unable to compile the program using CMAKE, send me an email, and I will send you a copy of the program compilable without CMAKE.

1. Decompress the project folder
2. Navigate to FOLDER_NAME/Project2/build
3. Right click, Open Terminal Here
4. Enter the command "cmake .." into the terminal
5. Use the command "make" into the terminal
6. Use the command ./project2 PA2_test.sql to run the project from the test file...

NOTE: If you get an error saying that the current cmake cache is different, delete the build folder, make a new folder of the same name, then try again.

The program has been succesfully re-downloaded and executed with the above instructions. Let me know if you have trouble.

GITHUB LINK: https://github.com/nathaniellipsey/CS-457/

Compiled and run on Linux MINT 20.
===============================

PROJECT DESIGN

This program organizes multiple databases by creating Linux directories using the <filename> library (C++17 or higher required!)
Tables are created using standard <ifstream> and <ostream> commands, with the <filename> library being used to navigate through database subfolders.
Database functions are all written under Database.h, while table functions are all listed under Table.h.

Updating the program to meet the new specifications was a difficult effort. While some of the code I wrote was certainly spaghetti (mostly due to myself running out of time) it would be a rather simple fix to edit it all to be functional again, and it fulfills the program requirements just fine.

This time around I needed to actually modify and read parts of the written tables in detail. Since editing and reading from the file with each command function was, to say the least, less than desirable, I made it so that the program temporarily stores the values not only in the text file but also in a struct where it is easier to access and modify. Then, when the program wishes to commit to a change, it simply writes the structs to the file.

Slight modifications to the test.sql file had to be made as well-- most notably, the casing on the table "Product" was not consistant, preventing the program from writing to file. I certainly stand to make some improvements to the project between now and the next assignment, but overall I'm quite proud of how the program turned out. Editing the program to work with command line functions took a lot of effort, and several different ideas and versions where attempted and discarded before I ended up on this.

Finally, the program now works by putting a file in as an arguement in the console-- no more need to type out the commands manually. Please let me know if you have any questions or comments. I gave the program one last run and it appears to be functioning as intended (see Output.png).

