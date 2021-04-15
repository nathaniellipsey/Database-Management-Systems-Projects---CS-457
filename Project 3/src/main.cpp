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

struct table
{
	string name;
	tableColumn tableData[99];
};

table tables[3];

void joinTwoTables(int table1, int table2)
{
	//find number of elements
	int elem_num = 0;
	for (int i=0; i<99; i++)
	{
		if (tables[table1].tableData[i].type == "\0")
		{
			//search for a row w/ no type and start there
			elem_num = i;
			break;
		}
	}

	for (int i=0; i<99; i++)
	{
		if (tables[table2].tableData[i].type != "\0")
		{
			tables[table1].tableData[elem_num + i].type = tables[table2].tableData[i].type;
			tables[table1].tableData[elem_num + i].name = tables[table2].tableData[i].name;

			for (int j=0; j<99; j++)
			{
				if (tables[table2].tableData[i].data[j] != "\0")
				{
					tables[table1].tableData[elem_num + i].data[j] = tables[table2].tableData[i].data[j];
				}
			}
		}
	}
}

int matchTable(string tableName)
{
	if (tables[0].name == tableName)
	{
		return 0;
	}
	else if (tables[1].name == tableName)
	{
		return 1;
	}
	else if (tables[2].name == tableName)
	{
		return 2;
	}
	return -1;
}

void writeTableToFile(string database, string tableName)
{
	// create a path in an OS agnostic manner
	fs::path dir_path = fs::path(".") / database;
	fs::directory_entry dir(dir_path);
						    
	// create a path to the file:
	std::string file_ = tableName + ".dat";
	fs::path filename = dir_path / file_;

	ofstream os(filename);

	if (os)
	{

		int _table = matchTable(tableName);

		for (int i=0; i<99; i++)
		{
			if (tables[_table].tableData[i].type != "\0")
			{
				os << tables[_table].tableData[i].type << " " << tables[_table].tableData[i].name;
				if (tables[_table].tableData[i+1].type != "\0")
				{
					os << " | ";
				}
			}
			else
			{
				break;
			}
		}

		for (int i=0; i<99; i++)
		{
			for (int j=0; j<99; j++)
			{
				if (tables[_table].tableData[j].data[i] != "\0")
				{
					if (j == 0)
					{
						os << endl << tables[_table].tableData[j].data[i];
					}
					else
					{
						os << "\t| " << tables[_table].tableData[j].data[i];
					}
				}

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

/*void getTableFromFile(string database, string tableName)
{
	//create a path in an OS agnostic manner
	fs::path dir_path = fs::path(".") / database;
	fs::directory_entry dir(dir_path);

	//create a path to the file
	std::string file_ = tableName + ".dat";
	fs::path filename = dir_path / file_;

	fstream os(filename);

	if (os)
	{
		for (int i=0; i<99; i++)
		{
			os >> tableData[i].type;
			if (tableData[i].type == "|" || tableData[i].type ==)
		}
	}
}*/

void insertIntoTable(string _tableName, int column, string value)
{
	//cout << "Debug: Request to insert " << value << " into table " << _tableName << endl;
	int _toUpdate = matchTable(_tableName);
	for (int i=0; i<99; i++)
	{
		if (tables[_toUpdate].tableData[column].data[i] == "\0")
		{
			tables[_toUpdate].tableData[column].data[i] = value;
			//cout << "Debug: Inserted value '" << value << "' into table " << _tableName << " at column " << column << ", row " << i << endl;
			break;
		}
	}
}

void writeTableToConsole(string database, string tableName)
{

	int _table = matchTable(tableName);

		for (int i=0; i<99; i++)
		{
			if (tables[_table].tableData[i].type != "\0")
			{
				cout << tables[_table].tableData[i].type << " " << tables[_table].tableData[i].name;
				if (tables[_table].tableData[i+1].type != "\0")
				{
					cout << " | ";
				}
			}
			else
			{
				break;
			}
		}

		for (int i=0; i<99; i++)
		{
			for (int j=0; j<99; j++)
			{
				if (tables[_table].tableData[j].data[i] != "\0")
				{
					if (j == 0)
					{
						cout << endl << tables[_table].tableData[j].data[i];
					}
					else
					{
						cout << "\t| " << tables[_table].tableData[j].data[i];
					}
				}

			}
		}

	cout << endl;
}

void selectStarFrom(string join)
{

	if (join == "inner")
	{
		for (int k=0; k<3; k++)
		{
			for (int i=0; i<99; i++)
			{
				if (tables[k].tableData[i].name == "\0")
				{
					break;
				}
				cout << tables[k].tableData[i].name << " " << tables[k].tableData[i].type;
				cout << " | ";
			}
		}

		cout << endl;
		
		for (int i=0; i<99; i++)
		{
			for (int j=0; j<99; j++)
			{
				if (tables[0].tableData[i].type != "\0" && tables[0].tableData[i].type == tables[1].tableData[j].type)
				{
					//if matched, column i and column j have the same type
					//cout << "Debug: Column " << i << " and column " << j << " have the same type: " << tables[0].tableData[i].type << endl;

					for (int k=0; k<99; k++)
					{
						for (int m=0; m<99; m++)
						{
							if (tables[0].tableData[i].data[k] != "\0" && tables[0].tableData[i].data[k] == tables[1].tableData[j].data[m])
							{
								//cout << "Debug: Matched table 1, column " << i << " row " << k << " with table 2, column " << j << " row " << m << endl;
								//print the column for both
								for (int l=i; l<99; l++)
								{
									if (tables[0].tableData[l].data[k] == "\0")
									{
										break;
									}
									cout << tables[0].tableData[l].data[k] << " | ";
								}
								for (int l=i; l<99; l++)
								{
									if (tables[1].tableData[l].data[m] == "\0")
									{
										break;
									}
									cout << tables[1].tableData[l].data[m] << " | ";
								}
								cout << endl;
							}
						}
						
					}
				}
			}
		}
	}
	else if (join == "outer")
	{
		for (int k=0; k<3; k++)
		{
			for (int i=0; i<99; i++)
			{
				if (tables[k].tableData[i].name == "\0")
				{
					break;
				}
				cout << tables[k].tableData[i].name << " " << tables[k].tableData[i].type;
				cout << " | ";
			}
		}

		cout << endl;
		
		for (int i=0; i<99; i++)
		{
			for (int j=0; j<99; j++)
			{
				if (tables[0].tableData[i].type != "\0" && tables[0].tableData[i].type == tables[1].tableData[j].type)
				{
					//if matched, column i and column j have the same type
					//cout << "Debug: Column " << i << " and column " << j << " have the same type: " << tables[0].tableData[i].type << endl;

					for (int k=0; k<99; k++)
					{
						for (int m=0; m<99; m++)
						{
							if (tables[0].tableData[i].data[k] != "\0" && tables[0].tableData[i].data[k] == tables[1].tableData[j].data[m])
							{
								//cout << "Debug: Matched table 1, column " << i << " row " << k << " with table 2, column " << j << " row " << m << endl;
								//print the column for both
								for (int l=i; l<99; l++)
								{
									if (tables[0].tableData[l].data[k] == "\0")
									{
										break;
									}
									cout << tables[0].tableData[l].data[k] << " | ";
								}
								for (int l=i; l<99; l++)
								{
									if (tables[1].tableData[l].data[m] == "\0")
									{
										break;
									}
									cout << tables[1].tableData[l].data[m] << " | ";
								}
								cout << endl;
							}
						}
						
					}
				}
			}
		}

		//print the unused column
		cout << tables[0].tableData[0].data[2] << " | " << tables[0].tableData[1].data[2] << " | |" << endl;
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
							    	int _table;

							    	if (tables[0].name == "\0")
							    	{
							    		tables[0].name = input[2];
							    		_table = 0;
							    	}
							    	else if (tables[1].name == "\0")
							    	{
							    		tables[1].name = input[2];
							    		_table = 1;
							    	}
							    	else if (tables[2].name == "\0")
							    	{
							    		tables[2].name = input[2];
							    		_table = 2;
							    	}

							    	//cout << "Debug: Using table " << _table << ", " << tables[_table].name << endl;

							    	for (int i=2; i<99; i++)
							    	{
							    		if(input[i].front() == *"(")
							    		{
							    			//remove first letter
							    			//cout << "Debug: Removing front letter '(' from: " << input[i] << endl;
							        		input[i] = input[i].substr(1, input[i].size()-1);
							    		}
							    		else if(input[i].back() == *"," || input[i].back() == *")")
							    		{
							    			//cout << "Debug: Removing back letter ','/')' from: " << input[i] << endl;
							    			//remove the last letter
							        		input[i] = input[i].substr(0, input[i].size() -1);
							    		}
							    		else if(input[i] == "\0")
							    		{
							    			break;
							    		}
							    	}

							    	tables[_table].tableData[0].name = input[3];
							    	tables[_table].tableData[0].type = input[4];

							    	if (input[5] != "\0" && input[6] != "\0")
							    	{
							    		tables[_table].tableData[1].name = input[5];
							    		tables[_table].tableData[1].type = input[6];	
							    	}
							    	if (input[7] != "\0" && input[8] != "\0")
							    	{
							    		tables[_table].tableData[2].name = input[7];
							    		tables[_table].tableData[2].type = input[8];	
							    	}

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
							if (input[5] != "\0")
							{
								if (input[6] == "outer")
								{
									selectStarFrom("outer");
								}
								else
								{
									selectStarFrom("inner");
								}
								
							}
							else
							{
								writeTableToConsole(dataBaseInUse, input[3]);
							}
							
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

						//find table
						int _table = matchTable(input[1]);

						for (int j=0; j<3; j++)
						{
							if (tables[_table].tableData[j].name == input[7])
							{
								target = j;
								break;
							}
						}

							//cout << "Searching table " << tableData[target].name << endl;

							//if the data table name matches the requested name...
							if (tables[_table].tableData[target].name == input[7])
							{
								for (int i=0; i<99; i++)
								{
									//if the data tables data is empty
									if (tables[_table].tableData[target].data[i] == "\0")
									{
										//cout << "Break." << endl;
										break;
									} //if the data tables data matches the requested data
									else if (tables[_table].tableData[target].data[i] == input[9])
									{
										//change where .name = input[3]
										for (int j=0; j<3; j++)
										{
											if (tables[_table].tableData[j].name == input[3])
											{
												tables[_table].tableData[j].data[i] = input[5];
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

						int _table = matchTable(input[2]);
						if (_table == -1)
						{
							cout << "Error: Table name couldn't be matched!" << endl;
						}

						int count = 0;
						if (input[5] == "=")
						{
							for (int i=0; i<3; i++)
							{
								if (tables[_table].tableData[i].name == input[4])
								{
									for (int j=0; j<99; j++)
									{
										if (tables[_table].tableData[i].data[j] != "\0")
										{
											//cout << "Compare table " << tableData[i].name << " value " << tableData[i].data[j] << " with " << input[6] << endl;
											if (tables[_table].tableData[i].data[j] == input[6])
											{
												//delete the record
												tables[_table].tableData[0].data[j] = "\0";
												tables[_table].tableData[1].data[j] = "\0";
												tables[_table].tableData[2].data[j] = "\0";
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
								if (tables[_table].tableData[i].name == input[4])
								{
									for (int j=0; j<99; j++)
									{
										if (tables[_table].tableData[i].data[j] != "\0")
										{
											a = stoi(tables[_table].tableData[i].data[j]);
											b = stoi(input[6]);
											//cout << "Compare table " << tableData[i].name << " value " << tableData[i].data[j] << " with " << input[6] << endl;
											if (a > b)
											{
												//delete the record
												tables[_table].tableData[0].data[j] = "\0";
												tables[_table].tableData[1].data[j] = "\0";
												tables[_table].tableData[2].data[j] = "\0";
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

					int _table = matchTable(input[2]);

					if (input[1] == "into" || input[1] == "INTO")
					{ 
							//remove values(
							input[3] = input[3].substr(7, input[3].size()-7);

							for (int i=2; i<99; i++)
							{
							    if(input[i].front() == *"(")
							    {
							    	//remove first letter
							    	//cout << "Debug: Removing front letter '(' from: " << input[i] << endl;
							        input[i] = input[i].substr(1, input[i].size()-1);
							    }
								else if(input[i].back() == *"," || input[i].back() == *")")
							    {
							    	//cout << "Debug: Removing back letter ','/')' from: " << input[i] << endl;
							    	//remove the last letter
							        input[i] = input[i].substr(0, input[i].size() -1);
							    }
							    else if(input[i] == "\0")
							    {
							    	break;
							    }
							}

							insertIntoTable(input[2], 0, input[3]);

							if (input[4] != "\0")
							{
								insertIntoTable(input[2], 1, input[4]);
							}
							if (input[5] != "\0")
							{
								insertIntoTable(input[2], 2, input[5]);
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