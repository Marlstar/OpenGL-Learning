#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <CUSTOM/tilemap.h>

#include <vector>
#include <map>
#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>

#include <OTHER/termcolor.hpp>

std::map<int, const char*> tileIDs = {
	{0, "dirt"},
	{1, "grass"}
};


class TilemapManager
{
public:
	char* currentTilemap;
	std::map< char*, std::vector<std::vector<int>> > tilemaps;

	void initTilemap(const char* name, const char* path)
	{
		// Check if there is already a tilemap by that name
		if (tilemaps.count((char*)name) == 0)
		{
			//SetConsoleTextAttribute(GetStdHandle( STD_OUTPUT_HANDLE ), 4);
			std::cout << colour::red << "ERROR initialising tilemap \"" << name << "\" | A tilemap with that name already exists" << colour::reset << std::endl;
			//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		}
	};

private:
	const char* tilemapPath;
	
};
class Tile
{
public:
	float coords[2];
	float x;
	float y;

	float texCoords[2];
	float tx;
	float ty;

	char* name;

	Tile(char* name_, float coords_[2], float texCoords_[2])
	{
		name = name_;
		
		coords[0] = coords_[0];
		coords[1] = coords_[1];
		x = coords[0];
		y = coords[1];

		texCoords[0] = texCoords_[0];
		texCoords[1] = texCoords_[1];
		tx = texCoords[0];
		ty = texCoords[1];
	}
};

class Tilemap
{
public:
	int width = 0;
	int height = 0;

	const char* name;
	const char* path;

	Tilemap(const char* name_, const char* path_)
	{
		name = name_;
		path = path_;

		readTilemapFromFile(path);
	}
	std::vector<std::vector<int>> rawTilemap;
	std::vector<std::vector<const char*>> tilemapByTileNames;
	std::vector<std::vector<Tile*>> tilemap;

	
private:
	//////////////////////////////////////////////////////////////////
	void readTilemapFromFile(const char* path)
	{
		// Reading raw
		std::ifstream tilemapFile;
		tilemapFile.open(path);

		int val;

		std::string rline;

		// Getting all the nums out of the tilemap
		while (std::getline(tilemapFile, rline))
		{
			std::vector<int> rowRaw;
			std::vector<const char*> rowByNames;
			std::vector<Tile*> row;

			std::istringstream lineReader(rline);
			while (lineReader)
			{
				lineReader >> val;

				if (!lineReader) { break; }
				rowRaw.push_back(val);
				// Tile names
				rowByNames.push_back(tileIDs[val]);

			}
			rawTilemap.push_back(rowRaw);
			tilemapByTileNames.push_back(rowByNames);
			tilemap.push_back(row);
		}
	}

	void initTiles()
	{
		
		for (int y = 0; y < height; y++)
		{	
			std::cout << std::endl;
			tilemap.push_back(std::vector<Tile*>{});
			for (int x = 0; x < width; x++)
			{
				// Init tile at that coord

				// Coords to pass in
				float c[2]  = { x,y };
				float tc[2] = { 1,1 }; // need to fix this to actually be texture coordinates. needs to be multiple vertices

				Tile* t = new Tile((char*)tilemapByTileNames[y][x], c, tc);
				std::cout << *t->name << " " << std::endl;
				//Tile t((char*)tilemapByTileNames[y][x], c, tc);
				tilemap[y].push_back(t);
			}
		}
	}

};

