//include header files
#include "../include/Database.h"
#include "../include/Table.h"

//include external libraries
#include <iostream>
#include <string>
#include <fstream>
#include <locale>

//namespace
using namespace std;

//function to convert a string to all uppercase letters
string shoutString(string & str)
{
	for (unsigned int i=0; i < str.length(); i++)
	{
		str[i] = toupper(str[i]);
	}
	return str;
}

struct tableColumn
{
    std::string type;
    std::string name;
    std::string data[99] = {"\0"};
};

tableColumn tableData[3];

void writeTableToFile(string database, string tableName)
{
	// create a path in an OS agnostic manner
	fs::path dir_path = fs::path(".") / database;
	fs::directory_entry dir(dir_path);
						    
	// create a path to your file:
	std::string file_ = tableName + ".dat";
	fs::path filename = dir_path / file_;

	ofstream os(filename);

	if (os)
	{
		os << tableData[0].type << " " << tableData[0].name << " | ";
		os << tableData[1].type << " " << tableData[1].name << " | ";
		os << tableData[2].type << " " << tableData[2].name;

		for (int i=0; i<99; i++)
		{
			if (tableData[0].data[i] != "\0")
			{
				os << endl << tableData[0].data[i] << "\t| " << tableData[1].data[i] << "\t| " << tableData[2].data[i];
			}
			
		}
		os << endl;

		os.close();	
	}
	else
	{
		cerr << "!Failed to open file after creation." << endl;
	}
}

void writeTableToConsole(string database, string tableName)
{
	cout << tableData[0].type << " " << tableData[0].name << " | ";
	cout << tableData[1].type << " " << tableData[1].name << " | ";
	cout << tableData[2].type << " " << tableData[2].name;

	for (int i=0; i<99; i++)
	{
		if (tableData[0].data[i] != "\0")
		{
			cout << endl << tableData[0].data[i] << "\t| " << tableData[1].data[i] << "\t| " << tableData[2].data[i];
		}
		
	}
	cout << endl;
}

//main function
int main(int argc, char *argv[])
{
	//forward credits
	cout << "CS457\tProject 2\tNathan Lipsey" << endl;

	//define forward variables
	string dataBaseInUse = "\0"; //the current database being used
	string input[50]; //inputs from the console/text file

	//if a file was provided, try to open it
	if (argc > 1)
	{
		fstream testfile;
		testfile.open(argv[1]);

		//file opens succesfully if this is 1
		if (testfile.is_open())
		{
			while(1) //no need for an exit condition; .EXIT returns
			{
				//set all of the inputs to a null char
				for (int i=0; i<50; i++)
				{
					input[i] = "\0";
				}

				//read in the first word
				testfile >> input[0];
				if (input[0] == ".EXIT")
				{
					//read in the first line, exit if its .EXIT
					cout << "All done." << endl;
					return 0;
				}
				else
				{
					//read until it reaches a semicolon
					if (input[0].back() != *";")
					{
						for (int i=1; i<50; i++)
						{
							testfile >> input[i];
							if (input[i].back() == *";")
							{
								//remove semicolon, exit loop
								input[i] = input[i].substr(0, input[i].size()-1);
								break;
							}
						}
					}
				}

				/***************COMMAND INPUTS****************/

				//convert to all caps so that inputs aren't case sensitive
				if (shoutString(input[0]) == "CREATE") /** CREATE COMMAND **/
				{
					if (shoutString(input[1]) == "DATABASE")
					{
						//create database
						createDatabase(input[2]);
					}
					else if (shoutString(input[1]) == "TABLE")
					{
						if (dataBaseInUse != "\0")
						{
							/* create table */
							/** CREATE TABLE Product (pid int, name varchar(20), price float) **/
						    /** [0]    [1]   [2]     [3]  [4]  [5]  [6]          [7]   [8]    **/

							    //theres probably a better way to do this, but I'm running short on time...
							    if (!tableExists(input[2], dataBaseInUse))
							    {

							        if(input[3].front() == *"(")
							        {
							        	//remove first letter
							        	input[3] = input[3].substr(1, input[3].size()-1);
							        }

							        tableData[0].name = input[3];

						            if(input[4].back() == *",")
							        {
							        	//remove the last letter
							        	input[4] = input[4].substr(0, input[4].size() -1);
							        }

							        tableData[0].type = input[4];

							        tableData[1].name = input[5];

							        if(input[6].back() == *",")
							        {
							        	//remove the last letter
							        	input[6] = input[6].substr(0, input[6].size() -1);
							        }

							        tableData[1].type = input[6];

							        tableData[2].name = input[7];

							        if(input[8].back() == *")")
							        {
							        	//remove the last letter
							        	input[8] = input[8].substr(0, input[8].size() -1);
							        }

							        tableData[2].type = input[8];

							        writeTableToFile(dataBaseInUse, input[2]);
							        cout << "Table " << input[2] << " created." << endl;
							    }
							    else
							    {
							        cerr << "!Could not create table " << input[2] << " because it already exists!" << std::endl;
							    }
						}
						else
						{
							cerr << "!No database in use, cannot create table" << endl;
						}
					}
					else
					{
						cerr << "!Invalid syntax. Usage: CREATE {DATABASE/TABLE} NAME" << endl;
					}
				}
				else if (shoutString(input[0]) == "USE") /** USE COMMAND **/
				{
					if (databaseExists(input[1]) == true)
					{
						cout << "Using database " << input[1] << endl;
						dataBaseInUse = input[1];
					}
					else
					{
						cerr << "! That database does not exist." << endl;
					}
				}
				else if (shoutString(input[0]) == "DROP") /** DROP COMMAND **/
				{
					if (shoutString(input[1]) == "DATABASE")
					{
						//make sure a deleted database can't be used
						if (input[2] == dataBaseInUse)
						{
							dataBaseInUse = "\0";
						}
						dropDatabase(input[2]);
					}
					else if (shoutString(input[1]) == "TABLE")
					{
						if (dataBaseInUse != "\0")
						{
							dropTable(input[2], dataBaseInUse);
						}
						else
						{
							cerr << "!No database in use, cannot delete table" << endl;
						}
					}
					else
					{
						cerr << "!Invalid syntax. Usage: CREATE {DATABASE/TABLE} NAME" << endl;
					}
				}
				else if (shoutString(input[0]) == "SELECT") /** SELECT COMMAND **/
				{
					if (dataBaseInUse != "\0")
					{
						if (input[1] == "*" && shoutString(input[2]) == "FROM")
						{
							//select * from Product;
							writeTableToConsole(dataBaseInUse, input[3]);
						}
						else
						{
							//select name, price from product where pid != 2;
							//[0]    [1]   [2]   [3]  [4]     [5]   [6] [7][8]
						}
					}
					else
					{
						cerr << "!No database in use, cannot delete table" << endl;
					}
					
				}
				else if (shoutString(input[0]) == "ALTER") /** ALTER COMMAND **/
				{
					/* code */
				}
				else if (shoutString(input[0]) == "UPDATE") /** UPDATE COMMAND **/
				{
					if (dataBaseInUse != "\0")
					{
						/* update Product set name = 'Gizmo' where name = 'SuperGizmo' */
						/* [0]    [1]     [2] [3] [4] [5]    [6]   [7] [8] [9]         */

						//update Product set price = 14.99 where name = 'Gizmo';

						int count = 0;

						//cout << "DEBUG OUTPUT: Seeking to replace " << input[5] << " where " << input[9] << endl;

						//set target
						int target = 0;

						for (int j=0; j<3; j++)
						{
							if (tableData[j].name == input[7])
							{
								target = j;
								break;
							}
						}

							//cout << "Searching table " << tableData[target].name << endl;

							//if the data table name matches the requested name...
							if (tableData[target].name == input[7])
							{
								for (int i=0; i<99; i++)
								{
									//if the data tables data is empty
									if (tableData[target].data[i] == "\0")
									{
										//cout << "Break." << endl;
										break;
									} //if the data tables data matches the requested data
									else if (tableData[target].data[i] == input[9])
									{
										//change where .name = input[3]
										for (int j=0; j<3; j++)
										{
											if (tableData[j].name == input[3])
											{
												tableData[j].data[i] = input[5];
											}
										}
										count++;
									}
									//cout << "Did not match " << input[9] << " with input at table " << tableData[target].name << " " << tableData[target].data[i] << endl; 
								}
							}

						if (count == 1)
							cout << "1 record modified." << endl;
						else
							cout << count << " records modified." << endl;

						writeTableToFile(dataBaseInUse, input[1]);
					}
					else
					{
						cerr << "!No database in use, cannot delete table" << endl;
					}
				}
				else if (shoutString(input[0]) == "DELETE")
				{
					if (dataBaseInUse != "\0")
					{
						/* delete from product where name = 'Gizmo'; */
						/* [0]    [1]  [2]     [3]   [4] [5] [6]     */

						int count = 0;
						if (input[5] == "=")
						{
							for (int i=0; i<3; i++)
							{
								if (tableData[i].name == input[4])
								{
									for (int j=0; j<99; j++)
									{
										if (tableData[i].data[j] != "\0")
										{
											//cout << "Compare table " << tableData[i].name << " value " << tableData[i].data[j] << " with " << input[6] << endl;
											if (tableData[i].data[j] == input[6])
											{
												//delete the record
												tableData[0].data[j] = "\0";
												tableData[1].data[j] = "\0";
												tableData[2].data[j] = "\0";
												count++;
											}
										}
									}
								}
							}
						}
						else if (input[5] == ">")
						{

							/* delete from product where price > 150; */
							/* [0]    [1]  [2]     [3]   [4]  [5] [6] */

							for (int i=0; i<3; i++)
							{
								int a, b;
								//if name = PRICE
								if (tableData[i].name == input[4])
								{
									for (int j=0; j<99; j++)
									{
										if (tableData[i].data[j] != "\0")
										{
											a = stoi(tableData[i].data[j]);
											b = stoi(input[6]);
											//cout << "Compare table " << tableData[i].name << " value " << tableData[i].data[j] << " with " << input[6] << endl;
											if (a > b)
											{
												//delete the record
												tableData[0].data[j] = "\0";
												tableData[1].data[j] = "\0";
												tableData[2].data[j] = "\0";
												count++;
											}
										}
									}
								}
							}
						}

						if (count == 1)
							cout << "1 record deleted." << endl;
						else
							cout << count << " records deleted." << endl;


						writeTableToFile(dataBaseInUse, input[2]);
					}
					else
					{
						cerr << "!No database in use, cannot delete table" << endl;
					}
				}
				else if (shoutString(input[0]) == "INSERT" || input[0] == "insert")
				{
					
					/** insert into Product values(1,'Gizmo',      19.99) **/
					/** [0]    [1]  [2]     [3]      [4]           [5]    **/

					if (input[1] == "into" || input[1] == "INTO")
					{ 
							//remove values(
							input[3] = input[3].substr(7, input[3].size()-7);

							if(input[3].back() == *",")
							{
							    //remove the last letter
							    input[3] = input[3].substr(0, input[3].size() -1);
							}

							//find first empty spot
							for (int i=0; i<99; i++)
							{
								if (tableData[0].data[i] == "\0")
								{
									//write to that spot
									tableData[0].data[i] = input[3];
									break;
								}
							}

						    if(input[4].back() == *",")
							{
							    //remove the last letter
							    input[4] = input[4].substr(0, input[4].size() -1);
							}

							//find first empty spot
							for (int i=0; i<99; i++)
							{
								if (tableData[1].data[i] == "\0")
								{
									//write to that spot
									tableData[1].data[i] = input[4];
									break;
								}
							}

							if(input[5].back() == *")")
							{
							    //remove the last letter
							    input[5] = input[5].substr(0, input[5].size() -1);
							}

							//find first empty spot
							for (int i=0; i<99; i++)
							{
								if (tableData[2].data[i] == "\0")
								{
									//write to that spot
									tableData[2].data[i] = input[5];
									break;
								}
							}

							//writeTableToConsole(dataBaseInUse, input[2]);
							writeTableToFile(dataBaseInUse, input[2]);

							cout << "1 new record inserted." << endl;
					}
					else
					{
						cerr << "!Invalid syntax" << endl;
					}
				}
				else if (input[0].front() == *"-")
				{
					//do nothing
				}
				else
				{
					cerr << "!Unknown command" << endl;
				}

				/**********************************************/

			}
		}
		else
		{
			//file did not open succesfully, abort.
			cerr << "!Couldn't open the provided file!" << endl;
			return 0;
		}
	}
	else
	{
		cerr << "!No file was provided." << endl;
		return 0;
	}
}