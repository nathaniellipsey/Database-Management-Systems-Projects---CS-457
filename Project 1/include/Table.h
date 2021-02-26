//table functions
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

bool tableExists(const std::string name, std::string database)
{
	fs::path dir_path = fs::path(".") / database;
	fs::directory_entry dir(dir_path);

	std::string file_ = name + ".dat";

	fs::path f{dir_path / file_};
	if (fs::exists(f))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void createTable(std::string name, std::string database, std::string dt_1, std::string data1, std::string dt_2, std::string data2)
{
	// create a path in an OS agnostic manner
    fs::path dir_path = fs::path(".") / database;
    fs::directory_entry dir(dir_path);
    {
        // create a path to your file:
        std::string file_ = name + ".dat";
        fs::path filename = dir_path / file_;

        if (!tableExists(name, database))
        {
        	std::ofstream os(filename);

        	std::cout << "Table " << name << " created." << std::endl;
        	if(os)
	            os << dt_1 << " " << data1 << " | " << dt_2 << " " << data2;
	        else 
	        {
	            std::cerr << "!Failed opening " << filename << " for writing" << std::endl;
	        }
        	os.close();
        }
        else
        {
        	std::cerr << "!Failed to create table " << name << " because it already exists." << std::endl;
        }
    }
}

void dropTable(std::string name, std::string database)
{
	if (tableExists(name, database))
	{
		fs::path dir_path = fs::path(".") / database;
		fs::directory_entry dir(dir_path);

		std::string file_ = name + ".dat";
		fs::path filename = dir_path / file_;

		fs::remove(filename);
		std::cout << "Table " << name << " deleted." << std::endl;
	}
	else
	{
		std::cerr << "!Failed to delete " << name << " because it does not exist." << std::endl;
	}
}

void alterTable(std::string name, std::string database, std::string var_name, std::string var_type)
{
	// create a path in an OS agnostic manner
    fs::path dir_path = fs::path(".") / database;
    fs::directory_entry dir(dir_path);
    {
        // create a path to your file:
        std::string file_ = name + ".dat";
        fs::path filename = dir_path / file_;

        if (tableExists(name, database))
        {
        	//read in the data
        	std::fstream readfile;
        	readfile.open(filename);
        	if (readfile.is_open())
        	{
        		std::string file_info;
        		getline(readfile, file_info);
        		readfile.close();
        		std::ofstream os(filename);
	        	if(os)
	        	{
		            os << file_info << " | " << var_name << " " << var_type;
		        	std:: cout << "Table " << name << " modified." << std::endl;
	        	}
		        else 
		        {
		            std::cerr << "!Failed opening " << filename << " for writing" << std::endl;
		        }
	        	os.close();
	        	}
        }
        else
        {
        	std::cerr << "!Failed to alter table " << name << " because it does not exist." << std::endl;
        }
    }
}

void selectFromTable(std::string name, std::string database)
{
	// create a path in an OS agnostic manner
    fs::path dir_path = fs::path(".") / database;
    fs::directory_entry dir(dir_path);
    {
        // create a path to your file:
        std::string file_ = name + ".dat";
        fs::path filename = dir_path / file_;

        if (tableExists(name, database))
        {
        	//read in the data
        	std::fstream readfile;
        	readfile.open(filename);
        	if (readfile.is_open())
        	{
        		std::string file_info;
        		getline(readfile, file_info);
        		readfile.close();
        		std::cout << file_info << std::endl;
        	}
        }
        else
        {
        	std::cerr << "!Failed to query from table " << name << " because it does not exist." << std::endl;
        }
    }
}


        	/*std::ofstream os(filename);
	        if(os)
	            os << "Hello world.\n";
	        else 
	        {
	            std::cerr << "failed opening " << filename << " for writing\n";
	        }*/

/*
void createTable(std::string name)
{
	std::ifstream outdata(name + ".dat");
	if (outdata.is_open()) //if the file exists
	{
		std::cerr << "!Failed to create database " << name << " because it already exists." << std::endl;
	}
	else
	{
		std::ofstream outdata (name + ".dat");
		std::cout << "Database " << name << " created." << std::endl;
		outdata.close();
	}
}
*/