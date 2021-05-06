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