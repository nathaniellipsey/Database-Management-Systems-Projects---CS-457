//g++ -Wall -std=c++17 -o proj1 main.cpp

#include "../include/Database.h"
#include "../include/Table.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;


string fetchInput()
{
	string _input;
	cin >> _input;
	if (std::cin.fail() || _input == "")
	{
		//error!
		std::cin.clear();
		if (_input == "")
		{
			return " ";
		}
		else
		{
			cerr << "!Input not valid." << endl;
		}
		return "ERROR";
	}
	else
	{
		return _input;
	}
}

int countArgs(string arguements[])
{
	int count;
	for (int i=0; i<9; i++)
	{
		if (arguements[i] != "\0")
		{
			count++;
		}
	}
	return count;
}

int main()
{
	cout << "CS457\tPA1\tNathan Lipsey" << endl;
	string _command;
	string _args[9];
	bool check;
	string databaseInUse = "\0";
	while (1)
	{
		//read in a string command
		cout << "> ";
		cout.flush();
		//set parameters to empty strings
		_command = "\0";
		for (int i=0; i<9; i++)
		{
			_args[i] = "\0";
		}
		_command = fetchInput();
		if ((_command.back() != *";") && _command != ".EXIT")
		{
			for (int i=0; i<9; i++)
			{
				_args[i] = fetchInput();
				if (_args[i].back() == *";")
				{
					break;
				}
			}
		}

		//read parameters until a semicolon is found
		if (_command.back() == *";" && _command != ".EXIT")
		{
			for (int i=0; i<0; i++)
			{
				_args[i] = fetchInput();
				if (_args[i].back() == *";")
				{
					break;
				}
			}
		}

		//check for errors
		for (int i=0; i<9; i++)
		{
			if (_args[i].back() == *";")
			{
				//chop off the semicolon
				_args[i] = _args[i].substr(0, _args[i].size()-1);
			}
		}
		//c++ does not support switches for strings, so while this is pretty janky code it works
		if (_command == ".EXIT")
		{
			cout << "All done." << endl;
			return 0;
		}
		else if (_command == "CREATE")
		{
			if (_args[0] == "DATABASE")
			{
				//create new database
				createDatabase(_args[1]);
			}
			else if (_args[0] == "TABLE")
			{
				//create a new table
				if (databaseInUse != "\0")
				{
					//_command = CREATE
					//_args[0] = TABLE
					//_args[1] = tb_1
					//_args[2] = (a3
					//_args[3] = float,
					//_args[4] = a4
					//_args[5] = char(20))

					//cut off the (
					if (_args[2].front() == *"(")
					{
						//remove the first (
						_args[2].erase(0, 1);
						if (_args[3].back() == *",")
						{
							//remove the ,
							_args[3] = _args[3].substr(0, _args[3].size()-1);
							if (_args[5].back() == *")")
							{
								//remove the back )
								_args[5] = _args[5].substr(0, _args[5].size()-1);
								createTable(_args[1], databaseInUse, _args[2], _args[3], _args[4], _args[5]);
							}
						}
					}
					else
					{
						cerr << "!Invalid args. Usage: CREATE TABLE TABLENAME ARG1 ARG2 ARG3 ARG4" << endl;
					}
				}
				else
				{
					cerr << "!You must CREATE and USE a database first." << endl;
				}
			}
				else
			{
				cerr << "!Unknown command. Use CREATE TABLE or CREATE DATABASE." << endl;
			}
		}
		else if (_command == "USE")
		{
			//open a database for using tables
			if (databaseExists(_args[0]) == true)
			{
				databaseInUse = _args[0];
				cout << "Using database " << _args[0] << endl;;
			}
			else
			{
				cerr << "Database does not exist." << endl;
			}
			
		}
		else if (_command == "DROP")
		{
			//delete a database or table
			if (_args[0] == "DATABASE")
			{
				//delete database
				dropDatabase(_args[1]);
				if (databaseInUse == _args[1])
				{
					//make sure a deleted database can't be used.
					databaseInUse = "\0";
				}
			}
			else if (_args[0] == "TABLE")
			{
				//delete table
				//create a new table
				if (databaseInUse != "\0")
				{
					dropTable(_args[1], databaseInUse);
				}
				else
				{
					cerr << "!You must CREATE and USE a database first." << endl;
				}
			}
			else
			{
				cerr << "!Unknown command. Use DROP TABLE or DROP DATABASE." << endl;
			}
		}
		else if (_command == "SELECT")
		{
			//select an element from a table
			if (_args[0] == "*" && _args[1] == "FROM")
			{
				if (databaseInUse != "\0")
				{
					selectFromTable(_args[2], databaseInUse);
				}
				else
				{
					cerr << "!You must CREATE and USE a database first." << endl;
				}
			}
			else
			{
				cerr << "!Invalid arguements. Usage: SELECT * FROM table_name;" << endl;
			}
		}
		else if (_command == "ALTER")
		{
			//alter a table to add an element
			if (_args[0] == "TABLE" && _args[2] == "ADD")
			{
				//alter table: ALTER TABLE table_name ADD a3 float;
				//_args[0] = TABLE
				//_args[1] = tbl_1
				//_args[2] = ADD
				//_args[3] = a3
				//_args[4] = float
				alterTable(_args[1], databaseInUse, _args[3], _args[4]);
			}
			else
			{
				cerr << "!Invalid arguements. Usage: ALTER TABLE table_name var_name var_type;" << endl;
			}
		}
		else
		{
			cerr << "!Unknown command: Use .EXIT, USE, DROP, SELECT, or ALTER." << endl;
		}
	}
}
