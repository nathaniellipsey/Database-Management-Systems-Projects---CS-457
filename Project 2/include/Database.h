//database functions
#pragma once
#include "Table.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

void createDatabase(const std::string name)
{
	try
	{
		if (fs::create_directory(name))
		{
			std::cout << "Database " << name << " created." << std::endl;
		}
		else
		{
			std::cerr << "!Failed to create database " << name << " because it already exists." << std::endl;
		}
	}
	catch (const std::exception& e)
	{ 
		std::cerr << e.what() << std::endl; 
	}
}

void dropDatabase(const std::string name)
{
	try
	{
		if (fs::remove_all(name))
		{
			std::cout << "Database " << name << " deleted." << std::endl;
		}
		else
		{
			std::cerr << "!Failed to delete " << name << " because it does not exist." << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

bool databaseExists(const std::string name)
{
	fs::path f{name};
	if (fs::exists(f))
	{
		return true;
	}
	else
	{
		return false;
	}
}