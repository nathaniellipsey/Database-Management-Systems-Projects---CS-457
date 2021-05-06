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

//forward declaration of functions
void writeTableToConsole(string database, string tableName);
void selectStarFrom(string join);
void executeTokens(string tokens[]);

//definitions
#define MAX_ROWS 99
#define MAX_COLUMNS 99
#define MAX_TOKENS 99

//global vars
string dataBaseInUse = "\0"; //makes note of which database is in use
string tableInUse = "\0"; //makes note of which table is in use
bool transaction_in_progress = false;
bool unsaved_changes = false;
string lock = "\0";

//define table column member
class tableMember
{
	protected:
		string memberName;
		string valueType;
		string tableValues[MAX_COLUMNS];
	public:
		tableMember()
		{
			//default const
			memberName = "\0";
			valueType = "\0";
			for (int i=0; i<MAX_COLUMNS; i++)
			{
				tableValues[i] = "\0";
			}
		}
		tableMember(const string mn, const string vt)
		{
			//param const
			memberName = mn;
			valueType = vt;
			for (int i=0; i<MAX_COLUMNS; i++)
			{
				tableValues[i] = "\0";
			}
		}
		void clearTableMember()
		{
			memberName = "\0";
			valueType = "\0";
			for (int i=0; i<MAX_COLUMNS; i++)
			{
				tableValues[i] = "\0";
			}
		}
		~tableMember()
		{
			//destructor
			clearTableMember();
		}
		bool hasStringValue(const string V)
		{

			for (int i=0; (tableValues[i]) != "\0"; i++)
			{
				if ((tableValues[i]) == V)
				{
					return true;
				}
			}

			return false;
		}
		string getMemberName()
		{
			return memberName;
		}
		string getValueType()
		{
			return valueType;
		}
		void setMemberName(const string n)
		{
			memberName = n;
		}
		void setValueType(const string s)
		{
			valueType = s;
		}
		string getTableValue(const int i)
		{
			return tableValues[i];
		}
		void setTableValue(const int i, string n)
		{
			tableValues[i] = n;
		}
		void pushValue(const string V)
		{
			for (int i=0; i<MAX_COLUMNS; i++)
			{
				if (tableValues[i] == "\0")
				{
					tableValues[i] = V;
					break;
				}
			}
		}
};

//Global variables

class TableC
{
	protected:
		string tableName;
	public:
		tableMember TM[MAX_ROWS];

		TableC()
		{
			//default const
			tableName = "\0";
		}
		string getTableName()
		{
			return tableName;
		}
		void setTableName(const string n)
		{
			tableName = n;
		}
		void printTableToConsole()
		{

			if (unsaved_changes == false)
			{
				loadTableFromFile(tableInUse, dataBaseInUse);
			}

			//print types
			for (int j=0; j<MAX_ROWS; j++)
			{
				if (TM[j].getMemberName() != "\0")
				{
					cout << TM[j].getMemberName() << " " << TM[j].getValueType();
					if (TM[j+1].getMemberName() != "\0")
					{
						//if this isn't the last table, add a " | "
						cout << " | ";
					}
				}
			}
			cout << "\n";

			for (int j=0; j<MAX_COLUMNS; j++)
			{
				for(int i=0; i<MAX_ROWS; i++)
				{
					if(TM[i].getMemberName() != "\0" && TM[i].getTableValue(j) != "\0")
					{
					//row exists, print:
						cout << TM[i].getTableValue(j) << "\t\t";
					}
					else
					{
						break;
					}
					if (TM[i+1].getMemberName() == "\0")
					{
						cout << "\n";
					}
				}
			}
		}
		bool printTableToFile(const string dname, const string tname)
		{
			/*** cout << "DEBUG: Call to printTableToFile().\n"; ***/

			//create a path in an OS agnostic manner
			fs::path dir_path = fs::path(".") / dname;
			fs::directory_entry dir(dir_path);

			//create a path to the file
			string _file = tname + ".dat";
			fs::path filename = dir_path / _file;

			ofstream os(filename);

			if(os)
			{
				//print types
				for (int j=0; j<MAX_ROWS; j++)
				{
					if (TM[j].getMemberName() != "\0")
					{
						os << TM[j].getMemberName() << " " << TM[j].getValueType();
						if (TM[j+1].getMemberName() != "\0")
						{
							//if this isn't the last table, add a " | "
							os << " | ";
						}
					}
				}
				os << "\n";

				for (int j=0; j<MAX_COLUMNS; j++)
				{
					for(int i=0; i<MAX_ROWS; i++)
					{
						if(TM[i].getMemberName() != "\0" && TM[i].getTableValue(j) != "\0")
						{
							//row exists, print:
							os << TM[i].getTableValue(j) << "\t\t";
						}
						else
						{
							break;
						}
						if (TM[i+1].getMemberName() == "\0")
						{
							os << "\n";
						}
					}
				}

				os.close();
				return true;
			}
			else
			{
				cerr << "!Failed to open file!" << endl;
				return false;
			}
		}
		bool createTableLock(string tname, string dname)
		{
			//create a path in an OS agnostic manner
			fs::path dir_path = fs::path(".") / dname;
			fs::directory_entry dir(dir_path);

			//create a path to the file
			string _file = tname + "_lock.dat";
			fs::path filename = dir_path / _file;

			ofstream os(filename);

			if(os)
			{
				/*** cout << "Created table lock named " << filename << endl; ***/
				os.close();
				lock = filename;
				return true;
			}
			cout << "Could not create table lock named " << filename << endl;
			return false;
		}
		bool removeTableLock()
		{
			if (tableExists(lock, dataBaseInUse))
			{
				dropTable(lock, dataBaseInUse);
				lock = "\0";
				return true;
			}
			else
			{
				return false;
			}
		}
		void useTable(const string tname)
		{
			if (tname != tableName)
			{
				//trying to use a table other than the one currently in use

			}
		}
		void loadTableFromFile(const string tname, const string dname)
		{
			if (unsaved_changes == true)
			{
				//do nothing-- don't override unsaved changes!
				/*** cout << "DEBUG: Did not override changes." << endl;  ***/
			}
			else
			{
				clearAll();

				if(tableExists(tname, dname))
				{
					//found the table; load it up
					//TODO: load the table from the file

					fs::path dir_path = fs::path(".") / dname;
					fs::directory_entry dir(dir_path);

					//create a path to the file
					string _file = tname + ".dat";
					fs::path filename = dir_path / _file;

					ifstream os(filename);

					string x;
					int z = 0;

					if(os)
					{
						while (os.peek() != '\n')
						{
							if (os >> x)
							{
								/*** cout << "DEBUG: Set name of column " << z << " to " << x << endl; ***/

								TM[z].setMemberName(x);
								if (os >> x)
								{
									/*** cout << "DEBUG: Set value type of column " << z << " to " << x << endl; ***/

									TM[z].setValueType(x);

									//skip the next word since its " | "
									if (os.peek() != '\n')
									{
										os >> x;
										/*** cout << "DEBUG: Skipped " << x << endl; ***/
									}
									/*** cout << "DEBUG: Moved on from column " << z << " to column " << z+1 << endl; ***/
									z++;
								}
							}
							else
							{
								break;
							}
						}
						/*** cout << "DEBUG: Peeked a new line.\n"; ***/

						/*** cout << "DEBUG: z = " << z << endl; ***/

						int i = 0;
						while (os >> x)
						{
							if (i == z)
							{
								i = 0;
							}

							TM[i].pushValue(x);
							i++;

							/*** cout << "DEBUG: Pushed value " << x << " onto column " << i << endl; ***/
						}

						/*** cout << "DEBUG: Loaded table from file." << endl; ***/
						tableInUse = tname;
					}

				}
				else
				{
					cerr << "!Table does not exist!" << "\n";
				}
			}
		}
		void clearAll()
		{
			tableName = "\0";
			tableInUse = "\0";
			for (int i=0; i<MAX_ROWS; i++)
			{
				TM[i].clearTableMember();
			}
			/** cout << "DEBUG: Table cleared.\n"; **/
		}
		void UpdateTable(string mname1, string newValue, string mname2, string w)
		{
			//function that commits the changes
			//update table_name set mname1 = newValue where mname2 = w;

			//use "printTableToFile" function to commit.
			int i = 0;
			int j = 0;

			//search for a row with the same name as mname1
			for (i=0; i<MAX_ROWS; i++)
			{
				if (TM[i].getMemberName() == mname1)
				{
					break;
				}
				else if (i == MAX_ROWS)
				{
					i = -1;
					cerr << "!Error: Row " << mname1 << " does not exist!\n";
				}
			}

			//search for a row with the same name as mname2
			for (j=0; j<MAX_ROWS; j++)
			{
				if (TM[j].getMemberName() == mname2)
				{
					break;
				}
				else if (j == MAX_ROWS)
				{
					j = -1;
					cerr << "!Error: Row " << mname2 << " does not exist!\n";
				}
			}

			//if both of these passed, the rows exist
			if (j != -1 && i != -1)
			{
				int x = 0;
				for (x=0; x<MAX_COLUMNS; x++)
				{
					//search mname2 for the value
					if (TM[j].getTableValue(x) == w)
					{
						//update the value, but don't save it to the file yet.

						/*** cout << "DEBUG: Replaced value " << TM[i].getTableValue(x) << " with value " << newValue << endl; ***/

						TM[i].setTableValue(x, newValue);
						unsaved_changes = true;
					}
				}
			}
		}
}; TableC T; //temporary table storage and access

int main(int argc, char *argv[])
{
	cout << "CS457\tProject 4\tNathan Lipsey" << endl;
	cout << "https://github.com/nathaniellipsey/CS-457" << endl;

	string input;
	string tokens[MAX_TOKENS];

	//if a test file was provided, open it.
	if (argc > 1)
	{
		fstream f;
		f.open(argv[1]);

		if(f.is_open())
		{
			cout << "Succesfully opened test file " << argv[1] << ".\n";
			while(1)
			{
				for (int i=0; i<MAX_TOKENS; i++)
				{
					tokens[i] = "\0";
				}

				//read in word by word
				f >> tokens[0];
				if (tokens[0] == ".EXIT")
				{
					cout << "All done." << endl;
					return 0;
				}
				else if (tokens[0].front() == *"-")
				{
					//If the line starts with "--", skip the line
					f.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				else
				{
					if (tokens[0].back() != *";")
					{
						for (int i=1; i<MAX_TOKENS; i++)
						{
							f >> tokens[i];
							if (tokens[i].back() == *";")
							{
								//remove semicolon, exit loop
								tokens[i] = tokens[i].substr(0, tokens[i].size()-1);
								break;
							}
						}
					}
					else
					{
						tokens[0] = tokens[0].substr(0, tokens[0].size()-1);
					}
					executeTokens(tokens);
				}
			}
		}
	}
	else
	{
			while(1)
			{
				for (int i=0; i<MAX_TOKENS; i++)
				{
					tokens[i] = "\0";
				}

				//read in word by word
				cin >> tokens[0];
				if (tokens[0] == ".EXIT")
				{
					cout << "All done." << endl;
					return 0;
				}
				else if (tokens[0].front() == *"-")
				{
					//If the line starts with "--", skip the line
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				else
				{
					if (tokens[0].back() != *";")
					{
						for (int i=1; i<MAX_TOKENS; i++)
						{
							cin >> tokens[i];
							if (tokens[i].back() == *";")
							{
								//remove semicolon, exit loop
								tokens[i] = tokens[i].substr(0, tokens[i].size()-1);
								break;
							}
						}
					}
					else
					{
						tokens[0] = tokens[0].substr(0, tokens[0].size()-1);
					}
					executeTokens(tokens);
				}
			}
	}

	return 0;
}

void executeTokens(string tokens[])
{
	if (tokens[0] == "CREATE")
	{
		//Database create function
		//Usage: CREATE DATABASE db_name;
		createDatabase(tokens[2]);
	}
	else if (tokens[0] == "USE")
	{
		//Databuse use function
		//Usage: USE db_name;
		if (databaseExists(tokens[1]))
		{
			if (transaction_in_progress == false)
			{
				dataBaseInUse = tokens[1];
				cout << "Using database " << tokens[1] << "." << endl;	
			}
			else
			{
				cout << "!You can't change databases while you are in a transaction." << endl;
			}
		}
		else
		{
			cerr << "!Could not use database " << tokens[1] << "because it doesn't exist!\n";
		}
	}
	else if (tokens[0] == "create")
	{
		//Table use function
		//Usage: create table table_name (data_name, data_type....);
		if (dataBaseInUse == "\0")
		{
			cerr << "!You need to use a database first. Usage: USE database_name;\n";
		}
		else if (tableInUse == tokens[2])
		{
			cerr << "!You can't create a table that is in use!\n";
		}
		else if (tableExists(tokens[2], dataBaseInUse))
		{
			cerr << "!A table with that name already exists in database " << dataBaseInUse << "!\n";
		}
		else
		{
			int y = 0;

			if (T.getTableName() != "\0")
			{
				//clear the current table
				T.clearAll();
			}
			//set the name to the second token
			T.setTableName(tokens[2]);
			tableInUse = tokens[2];

			//check for the "("
			if (tokens[3].front() == *"(")
			{
				//remove the "("
				tokens[3] = tokens[3].substr(1, tokens[3].size()-1);

				//add to name, add to type; repeat until it reaches a ")" instead of a comma
				for (int i=3; i<MAX_TOKENS-3; i++)
				{
					//odd numbers are names, even numbers are types
					if (i%2 != 0) //if odd
					{
						T.TM[y].setMemberName(tokens[i]); //set the data name
					}
					else
					{
						if (tokens[i].back() == *",")
						{
							//continue looping
							tokens[i] = tokens[i].substr(0, tokens[i].size() -1);
							T.TM[y].setValueType(tokens[i]);
							y++;
						}
						else if (tokens[i].back() == *")")
						{
							//stop the loop, a ")" was found
							tokens[i] = tokens[i].substr(0, tokens[i].size() -1);
							T.TM[y].setValueType(tokens[i]);
							break;
						}
						else
						{
							cerr << "!Token missing paranetheses or commas! Usage: create table table_name (data_name, data_type....);\n";
							y = -1;
							break;
						}
					}
				}

				if (y == -1) //set to this value if there was an error
				{
					T.clearAll();
				}
				else
				{
					T.printTableToFile(dataBaseInUse, T.getTableName());
					cout << "Table " << T.getTableName() << " created.\n";
				}
			}
			else
			{
				cerr << "!Error: Token missing paranetheses! Usage: create table table_name (data_name, data_type....);\n";
			}
		}
	}
	else if (tokens[0] == "insert")
	{
		//Table insert function
		//Usage: insert into table_name values (value1, value2....);
		if (dataBaseInUse == "\0")
		{
			cerr << "!Error: You have to USE a database first. Syntax: USE database_name;\n";
		}
		else if (!tableExists(tokens[2], dataBaseInUse))
		{
			cerr << "!Error: Table " << tokens[2] << " doesn't exist in database " << dataBaseInUse << ".\n";
		}
		else if (tableExists(tokens[2] + "_lock", dataBaseInUse))
		{
			cerr << "!Error: Table " << tokens[2] << " is locked!\n";
		}
		else
		{
			if (tableInUse != tokens[2])
			{
				//if the selected table isn't being used, set to that table
				T.loadTableFromFile(tokens[2], dataBaseInUse);
			}
			if (tokens[4].front() == *"(")
			{
				int y = 0;

				//remove the "("
				tokens[4] = tokens[4].substr(1, tokens[4].size()-1);

				//add to name, add to type; repeat until it reaches a ")" instead of a comma
				for (int i=4; i<MAX_TOKENS-4; i++)
				{
					if (tokens[i].back() == *")")
					{
						tokens[i] = tokens[i].substr(0, tokens[i].size() -1);
						T.TM[y].pushValue(tokens[i]);
						break;
					}
					else if (tokens[i].back() == *",")
					{
						tokens[i] = tokens[i].substr(0, tokens[i].size() -1);
						T.TM[y].pushValue(tokens[i]);
						y++;
					}
					else
					{
						cerr << "!Error: Unrecognized syntax. USAGE: insert into table_name values (value1, value2....)\n";
						y = -1;
					}
				}

				if (y != -1) //set to this value if there was an error
				{
					T.printTableToFile(dataBaseInUse, T.getTableName());
					cout << "Inserted new values.\n";
				}
			}
			else
			{
				cerr << "!Error: Token missing paranetheses!;\n";
			}
		}

	}
	else if (tokens[0] == "begin")
	{
		//Transaction function
		//Usage: begin transaction;
		if (tokens[1] == "transaction" && tokens[2] == "\0")
		{
			if (transaction_in_progress == false)
			{
				cout << "Transaction starts." << endl;
				transaction_in_progress = true;

				if (!tableExists(tableInUse + "_lock", dataBaseInUse))
				{
					T.createTableLock(tableInUse, dataBaseInUse);
				}
			}
			else
			{
				cout << "!A transaction is already in progress." << endl;
			}
		}
		else
		{
			cerr << "!Error: Unrecognized syntax. USAGE: begin transaction;\n";
		}
	}
	else if (tokens[0] == "update")
	{
		//Update table function
		//Usage: update table_name set data_name = data_type where data_name = data_type
		if (dataBaseInUse == "\0")
		{
			cerr << "!Error: You have to USE a database first. Syntax: USE database_name;\n";
		}
		else if (!tableExists(tokens[1], dataBaseInUse))
		{
			cerr << "!Error: Table " << tokens[1] << " doesn't exist in database " << dataBaseInUse << ".\n";
		}
		else
		{
			if (tableInUse != tokens[1])
			{
				//if the selected table isn't being used, set to that table
				T.loadTableFromFile(tokens[1], dataBaseInUse);
			}
			T.UpdateTable(tokens[3], tokens[5], tokens[7], tokens[9]);
			unsaved_changes = true;
			cout << "1 record modified.\n";
		}
		
	}
	else if (tokens[0] == "select")
	{
		//Select function
		//Functionality only requires "select * from" for this project
		//For Select data_name from data_list, see project 3.
		//Usage: select * from table_name

		if (tableInUse != tokens[3])
		{
			if (dataBaseInUse != "\0")
			{
				T.loadTableFromFile(tokens[3], dataBaseInUse);
			}
			else
			{
				cout << "USE a database first." << endl;
			}
			
		}

		T.printTableToConsole();
	}
	else if (tokens[0] == "commit")
	{
		//Commit function
		//Usage: commit;
		if (unsaved_changes == false)
		{
			cout << "Cannot commit; no changes have been made." << endl;
		}
		else if (transaction_in_progress == false)
		{
			cout << "There is no transaction in progress." << endl;
		}
		else if (tableExists(tokens[2] + "_lock", dataBaseInUse))
		{
			cerr << "!Error: Table " << tableInUse << " is locked!\n";
			cout << "Transaction abort." << endl;
			unsaved_changes = false;
		}
		else
		{
			if (lock != "\0")
			{
				if (!T.printTableToFile(dataBaseInUse, tableInUse))
				{
					cout << "Transaction abort." << endl;
					transaction_in_progress = false;
				}
				else
				{
					if (tableInUse == "\0")
					{
						cout << "No table in use." << endl;
					}

					if (T.printTableToFile(dataBaseInUse, tableInUse))
					{

						/************* DEBUG **************/
						/*** T.printTableToConsole(); ***/

						unsaved_changes = false;
						transaction_in_progress = false;
						T.removeTableLock();
						cout << "Transaction committed." << endl;
					}
					else
					{
						cout << "Transaction abort." << endl;
						T.removeTableLock();
						transaction_in_progress = false;
					}	
				}
			}
			else
			{
				cerr << "!Error: Table " << tableInUse << " is locked!\n";
				cout << "Transaction abort." << endl;
				unsaved_changes = false;
			}
		}
	}
	else
	{
		if (tokens[0] != "--")
		{
			cout << "!Error: Command not recognized.\n";
		}
	}
}