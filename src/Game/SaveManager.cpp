/*
** EPITECH PROJECT, 2018
** bomberman
** File description:
** SaveManager.cpp
*/

#ifdef __linux__
#include <dirent.h>
#elif _WIN32
#include <windows.h>
#endif
#include <fstream>
#include <sys/stat.h>
#include "../../include/Game/SaveManager.hpp"
#include "../../include/Game/EntityFactory.hpp"
#include "../../include/PathManager.hpp"

void SaveManager::save(EntitiesMap &map, const std::string &filename)
{
	#ifdef _WIN32
		CreateDirectory(PathManager::getHomePath("save").c_str(), nullptr);
	#elif __linux__
		mkdir(PathManager::getHomePath("save").c_str(), 0733);
	#endif

	std::ofstream file(PathManager::getHomePath(filename),
			   std::ofstream::out);
	std::cout << PathManager::getHomePath(filename) << std::endl;
	if (!file.good())
		throw std::runtime_error("Can't save the Entities");
	for (auto y = 0 ; y < map.getMap().size() ; ++y)
		for (auto x = 0 ; x < map.getMap()[y].size() ; ++x)
			for (auto &e: map.getMap()[y][x])
				file << *e;
}

void SaveManager::load(EntitiesMap &pool, const std::string &filename)
{
	std::ifstream file(filename, std::ifstream::in);
	if (!file.good())
		throw std::runtime_error("Can't load the Entities");
	std::string name;
	EntityFactory fact;
	while (std::getline(file, name, '\0')) {
		auto e = fact.createEntity(name);
		file >> *e;
		pool.insert(e.get(), {e->getPosX(), e->getPosY()});
		e.release();
	}
}
